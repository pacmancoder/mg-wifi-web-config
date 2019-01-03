Mongoose OS WiFi Web Config (MGWWC)
==========================================================

This library for Mongoose OS will help you to configure credentials
for wifi in sta mode using ap mode to provide web interface for
configuration purposes

# Motivation
MGWWC is inspired by [wifi-setup-web-ui](https://github.com/mongoose-os-libs/wifi-setup-web-ui)
library, but instead of relating on RPC and http-server library it uses only core Mongoose OS
functionality - wifi library and mongoose network library.

All WebUI frontend were minimized to acheive minimal library footprint

# Features
- Small footprint
    - RPC is not required
    - http-server library is not required
    - Tiny WebUI frontend
    - Tiny C++ backend
    - Minimized frontend files
- Flexible
    - All library files are prefixed with `__mgwwc_` so they shouldn't
      interfere with existing files in your project's fs
    - Can be triggered manually or during boot
    - Turns ON AP automatically when triggered
    - Provides handlers for user actions, which can be intercepted and validated
        - Example 1: validate that provided password is at least 16 characters long
        - Example 2: reboot device after user entered credentials
    - After configuration is completed, WebUI autolaunch can be disabled
      right in the WebUI itself
- Supported languages
    - C/C++
- Easy to use

# How to use
- Add library to your project dependencies
- Change AP settings mos.yml if required
- Flash firmware
- Connect to AP with any device (Default ssid is Mongoose\_<MAC ADDRESS>. Default password is Mongoose)
- Navigate to AP ip (192.168.4.1 by default)
- Use WebUI
- Done!

# How to rebuild fs
For normal build just invoke `mos --platform <platform> build`
But if any files from `src-frontend` directory were cahnged, please invoke `rebuild_fs.sh` first.

If `rebuild_fs.sh` will be invoked, please be sure that `purify-css`, `html-minifier` and `uglify-js` npm packages were instaled globally in the system.

# Third-party code
- [lit.js/util.js](https://github.com/ajusa/lit) frameworks

# TODO
- Add mJS support
- Implement callback unregister logic
- Add WebUI buttons to enable/disable ap/sta

