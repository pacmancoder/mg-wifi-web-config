[Mongoose OS] - WiFi credential manager with web interface
==========================================================

This library for Mongoose OS will help you to configure credentials
for wifi in sta mode using ap mode to provide web interface for
configuration purposes


# Features
- Small footprint
    - RPC is not required
    - Tiny HTML frontend
    - Tiny C backend
- Flexible
    - Library files are placed under suparate folder `/system/xyz.pacmancoder.mg-wifi-web-config`
      so they shouldn't interfere with existing files in your project
    - Configurable default login and password for web interface
    - Web interface login and password could be changed in runtime
    - Can be triggered manually or during boot
    - Provides handlers for user actions, which can be intercepted and validated
        - Example 1: validate that provided password is at least 16 characters long
        - Example 2: reboot device after user entered credentials

# How to build
For normal build just invoke `mos --platform <platform> build`  
But if any files from `src-frontend` directory were cahnged, please invoke `rebuild_fs.sh` first.

If `rebuild_fs.sh` will be invoked, please be sure that `purifycss`, `html-minifier` and `uglify-js` npm packages were instaled globally in the system.

# Third-party code
- [lit.js/util.js](https://github.com/ajusa/lit) frameworks

