#include "mgos_config.h"
#include "mgos_mongoose.h"

#include <iostream>
#include <sstream>
#include <string>

#include <mgwwc/core.h>
#include <mgwwc/callbacks.h>


namespace
{
    // TODO: change
    const char* files_path = "/system/xyz.pacmancoder.mg-wifi-web-config";

    const int HTTP_STATUS_SUCCESS = 200;
    const int HTTP_STATUS_FAILURE = 500;
    
    const mg_str msg_success_configure_sta = MG_MK_STR("SUCCESS: STA credentials have been changed");
    const mg_str msg_success_configure_ap = MG_MK_STR("SUCCESS: AP credentials have been changed");
    const mg_str msg_success_disable_webui = MG_MK_STR("SUCCESS: WebUI has been disabled");

    mgwwc_before_action_callback before_configure_sta_callback = nullptr;
    mgwwc_before_action_callback before_configure_ap_callback = nullptr;
    mgwwc_before_action_callback before_disable_webui_callback = nullptr;

    mgwwc_after_action_callback after_configure_sta_callback = nullptr;
    mgwwc_after_action_callback after_configure_ap_callback = nullptr;
    mgwwc_after_action_callback after_disable_webui_callback = nullptr;

    void* before_configure_sta_callback_user_data = nullptr;
    void* before_configure_ap_callback_user_data = nullptr;
    void* before_disable_webui_callback_user_data = nullptr;

    void* after_configure_sta_callback_user_data = nullptr;
    void* after_configure_ap_callback_user_data = nullptr;
    void* after_disable_webui_callback_user_data = nullptr;

    bool is_server_active = false;

     mg_serve_http_opts s_http_server_opts;
}

namespace
{
    struct Credentials
    {
        std::string login;
        std::string password;

        mg_str get_login_as_mg_str() { return mg_mk_str_n(login.data(), login.size()); };
        mg_str get_password_as_mg_str() { return mg_mk_str_n(login.data(), login.size()); };
    };

    Credentials parse_credentials(mg_str body)
    {
        Credentials c = {};
        
        std::stringstream stream(std::string(body.p, body.p + body.len));
        
        std::getline(stream, c.login);
        std::getline(stream, c.password);
        
        return c;
    }
    
    int to_http_status(mgwwc_result_status status)
    {
        if (status == MGWWC_STATUS_FAILURE)
        {
            return HTTP_STATUS_FAILURE;
        }
        
        return HTTP_STATUS_SUCCESS;
    }

    void send_response(mg_connection *c, mgwwc_result& result)
    {
        mg_send_head(c, to_http_status(result.status), result.message.len, nullptr);
        mg_send(c, result.message.p, result.message.len);
    }

    mgwwc_result invoke_before_configure_sta_callback(Credentials c)
    {
        if (before_configure_sta_callback == nullptr)
        {
            return MGWWC_RESULT_SUCCESS(msg_success_configure_sta);
        }

        mgwwc_action_data_configure_sta data = {
            c.get_login_as_mg_str(),
            c.get_password_as_mg_str()
        };

        return before_configure_sta_callback(&data, before_configure_sta_callback_user_data);
    }

    mgwwc_result invoke_before_configure_ap_callback(Credentials c)
    {
        if (before_configure_ap_callback == nullptr)
        {
            return MGWWC_RESULT_SUCCESS(msg_success_configure_ap);
        }

        mgwwc_action_data_configure_ap data = {
            c.get_login_as_mg_str(),
            c.get_password_as_mg_str()
        };

        return before_configure_ap_callback(&data, before_configure_ap_callback_user_data);
    }

    mgwwc_result invoke_before_disable_webui_callback()
    {
        if (before_disable_webui_callback == nullptr)
        {
            return MGWWC_RESULT_SUCCESS(msg_success_disable_webui);
        }

        return before_disable_webui_callback(nullptr, before_configure_ap_callback_user_data);
    }

    mgwwc_result invoke_after_configure_sta_callback(mgwwc_result& result)
    {
        if (after_configure_sta_callback != nullptr)
        {
            return after_configure_sta_callback(result, after_configure_sta_callback_user_data);
        }

        return result;
    }

    mgwwc_result invoke_after_configure_ap_callback(mgwwc_result& result)
    {
        if (after_configure_ap_callback != nullptr)
        {
            return after_configure_ap_callback(result, after_configure_ap_callback_user_data);
        }

        return result;
    }

    mgwwc_result invoke_after_disable_webui_callback(mgwwc_result& result)
    {
        if (after_disable_webui_callback != nullptr)
        {
            return after_disable_webui_callback(result, after_configure_ap_callback_user_data);
        }

        return result;
    }

    void set_sta_credentials(Credentials c)
    {
        std::cout << "STA credentials changed! SSID: " << c.login << std::endl;
        mgos_sys_config_set_wifi_sta_ssid(c.login.c_str());
        mgos_sys_config_set_wifi_sta_pass(c.password.c_str());
    }

    void set_ap_credentials(Credentials c)
    {
        std::cout << "AP credentials changed! SSID: " << c.login << std::endl;
        mgos_sys_config_set_wifi_ap_ssid(c.login.c_str());
        mgos_sys_config_set_wifi_ap_pass(c.password.c_str());
    }

    void disable_webui()
    {
        std::cout << "MGWWC WebUI Disabled" << std::endl;
        mgos_sys_config_set_wifi_ap_enable(false);
    }

    std::string get_bind_addr()
    {
        static const std::string PORT = "8266";
        const std::string ADDRESS = mgos_sys_config_get_wifi_ap_ip();

        return ADDRESS + ":" + PORT;
    }

    void http_event_handler(mg_connection* c, http_message* message)
    {
        if (mg_vcmp(&message->method, "GET") == 0)
        {
            mg_serve_http(c, message, s_http_server_opts);
        }
        else if(mg_vcmp(&message->method, "POST") == 0)
        {
            if (mg_vcmp(&message->uri, "/api/set_sta_credentials") == 0)
            {
                auto credentials = parse_credentials(message->body);
                auto result = invoke_before_configure_sta_callback(credentials);
                
                if (result.status == MGWWC_STATUS_SUCCESS)
                {
                    set_sta_credentials(credentials);
                }

                invoke_after_configure_sta_callback(result);
                
                send_response(c, result);
            }
            else if (mg_vcmp(&message->uri, "/api/set_ap_credentials") == 0)
            {
                auto credentials = parse_credentials(message->body);
                auto result = invoke_before_configure_ap_callback(credentials);
                
                if (result.status == MGWWC_STATUS_SUCCESS)
                {
                    set_ap_credentials(credentials);
                }
                
                invoke_after_configure_ap_callback(result);

                send_response(c, result);
            }
            else if (mg_vcmp(&message->uri, "/api/disable_webui") == 0)
            {
                auto result = invoke_before_disable_webui_callback();
                
                if (result.status == MGWWC_STATUS_SUCCESS)
                {
                    disable_webui();
                }

                invoke_after_disable_webui_callback(result);
                
                send_response(c, result);
            }
        }
    }

    extern "C" void event_handler(mg_connection *c, int ev, void *ev_data, void*)
    {
        if (ev == MG_EV_HTTP_REQUEST)
        {
            http_message* message = reinterpret_cast<http_message*>(ev_data);

            http_event_handler(c, message);
        }
        else if (ev == MG_EV_CLOSE)
        {
            is_server_active = false;
        }
    }

    void start_service(mg_mgr* mgr)
    {
        mgos_sys_config_set_wifi_ap_enable(true);

        mg_connection *nc = mg_bind(mgr, get_bind_addr().c_str(), event_handler, nullptr);
        mg_set_protocol_http_websocket(nc);

        s_http_server_opts.document_root = files_path;
        s_http_server_opts.enable_directory_listing = "yes";
    }
}

extern "C"
{
    // Callbacks API

    void mgwwc_register_before_action_callback(
            mgwwc_action action,
            mgwwc_before_action_callback callback,
            void* user_data
    ) {
        switch(action)
        {
            case MGWWC_ACTION_CONFIGURE_STA:
            {
                before_configure_sta_callback = callback;
                before_configure_sta_callback_user_data = user_data;
                return;
            }
            case MGWWC_ACTION_CONFIGURE_AP:
            {
                before_configure_ap_callback = callback;
                before_configure_ap_callback_user_data = user_data;
                return;
            }
            case MGWWC_ACTION_DISABLE_WEB_UI:
            {
                before_disable_webui_callback = callback;
                before_disable_webui_callback_user_data = user_data;
                return;
            }
        }
    }

    void mgwwc_register_after_action_callback(
            mgwwc_action action,
            mgwwc_after_action_callback callback,
            void* user_data
    ) {
        switch(action)
        {
            case MGWWC_ACTION_CONFIGURE_STA:
            {
                after_configure_sta_callback = callback;
                after_configure_sta_callback_user_data = user_data;
                return;
            }
            case MGWWC_ACTION_CONFIGURE_AP:
            {
                after_configure_ap_callback = callback;
                after_configure_ap_callback_user_data = user_data;
                return;
            }
            case MGWWC_ACTION_DISABLE_WEB_UI:
            {
                after_disable_webui_callback = callback;
                after_disable_webui_callback_user_data = user_data;
                return;
            }
        }
    }

    // Core API

    void mgwwc_trigger()
    {
        if (!is_server_active)
        {
            start_service(mgos_get_mgr());
        }
    }

    // Main library initialization function    

    bool mgos_pwm_init(void)
    {
        if (mgos_sys_config_get_mgwwc_trigger_on_boot())
        {
            mgwwc_trigger();
        }

        return true;
    }

}
