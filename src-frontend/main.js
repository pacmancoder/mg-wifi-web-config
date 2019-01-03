function make_credentials_for(kind)
{
    return document.getElementById(kind + "_login").value
        + "\n"
        + document.getElementById(kind + "_password").value;
}

function send_request(url, body)
{
    fetch(url, {
        method : "POST",
        body: body,
    }).then(function(response) {
        response.text().then(function(msg) {
            alert(msg)
        });
    });
}

function configure_sta()
{
    send_request("/api/set_sta_credentials", make_credentials_for("sta"));
}

function configure_ap()
{
    send_request("/api/set_ap_credentials", make_credentials_for("ap"));
}

function disable_webui()
{
    send_request("/api/disable_webui", null);
}