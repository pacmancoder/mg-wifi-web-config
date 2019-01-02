function make_credentials_for(kind)
{
    return document.getElementById(kind + "_login").value
        + "\n"
        + document.getElementById(kind + "_password").value;
}

function send_request(url, body, msg)
{
    fetch(url, {
        method : "POST",
        body: body,
    }).then(function(response) {
        alert(msg + ". Please reboot the device.");
    });
}

function configure_sta()
{
    send_request("/api/set_sta_credentials", make_credentials_for("sta"), "STA Credentials changed");
}

function configure_ap()
{
    send_request("/api/set_ap_credentials", make_credentials_for("ap"), "AP Credentials changed");
}

function disable_webui()
{
    send_request("/api/disable_webui", null, "Web UI disabled");
}