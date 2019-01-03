#pragma once

#include "mongoose.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Represents wifi web config callback status code
typedef enum
{
    MGWWC_STATUS_SUCCESS,
    MGWWC_STATUS_FAILURE,
    MGWWC_STATUS_INTERCEPTED
} mgwwc_result_status;

typedef enum
{
    MGWWC_ACTION_CONFIGURE_STA,
    MGWWC_ACTION_CONFIGURE_AP,
    MGWWC_ACTION_DISABLE_WEB_UI
} mgwwc_action;

typedef struct
{
    struct mg_str login;
    struct mg_str password;
} mgwwc_credentials;

typedef mgwwc_credentials mgwwc_action_data_configure_sta;
typedef mgwwc_credentials mgwwc_action_data_configure_ap;

/// Represents pair of status code and message, which
/// returned to user in the WebUI.
///
/// Please do not construct directrly; Use WIFI_WEB_CONFIG_*
/// macros instead
typedef struct
{
    mgwwc_result_status status;
    struct mg_str message;
} mgwwc_result;

/// Constructs result object, which signals successfull completition with provided message `msg`
///
/// If returned from callback then shceduled action will be completed, and requested action
/// will be marked as failed in the WebUI.
#define MGWWC_RESULT_SUCCESS(msg) mgwwc_result { MGWWC_STATUS_SUCCESS, msg }

/// Constructs result object, which signals that performed action has been failed and shows
/// error with provided message `msg`.
///
/// If returned from callback then sheeduled action will be canceled, and requested action
/// will be marked as failed in the WebUI.
#define MGWWC_RESULT_FAILURE(msg) mgwwc_result { MGWWC_STATUS_FAILURE, msg }

/// Constructs result object, which signals that performed action has been intercepted by
/// user code. Message `msg` will be shown in the WebUI
///
/// If returned from callback then shceduled action will be canceled, but requested action
/// will be marked as succeeded in the WebUI.
#define MGWWC_STATUS_INTERCEPTED(msg) mgwwc_result { MGWWC_STATUS_INTERCEPTED, msg }


/// Action data to action type bindings for callback:
///     MGWWC_ACTION_CONFIGURE_STA => struct mgwwc_action_data_configure_sta
///     MGWWC_ACTION_CONFIGURE_AP => struct mgwwc_action_data_configure_ap
///     MGWWC_ACTION_DISABLE_WEB_UI => NULL
typedef mgwwc_result (*mgwwc_before_action_callback)(void* action_data, void* user_data);

typedef mgwwc_result (*mgwwc_after_action_callback)(mgwwc_result result, void* user_data);

/// Registers callback which will be invoked before specified action.
/// Callback should return result, which will affect underlying action execution
void mgwwc_register_before_action_callback(
        mgwwc_action action,
        mgwwc_before_action_callback callback,
        void* user_data
);

/// Registers callback which will be invoked after specified action.
/// Callback could check action execution result or/and modify it to change
/// shown response in the WebUI 
void mgwwc_register_after_action_callback(
        mgwwc_action action,
        mgwwc_after_action_callback callback,
        void* user_data
);

#ifdef __cplusplus
}
#endif