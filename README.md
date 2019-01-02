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
