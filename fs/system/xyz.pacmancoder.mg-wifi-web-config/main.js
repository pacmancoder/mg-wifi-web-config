function e(e){return document.getElementById(e+"_login").value+"\n"+document.getElementById(e+"_password").value}function n(e,n,t){fetch(e,{method:"POST",body:n}).then(function(e){alert(t+". Please reboot the device.")})}function configure_sta(){n("/api/set_sta_credentials",e("sta"),"STA Credentials changed")}function configure_ap(){n("/api/set_ap_credentials",e("ap"),"AP Credentials changed")}function disable_webui(){n("/api/disable_webui",null,"Web UI disabled")}