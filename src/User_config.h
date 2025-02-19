#ifndef user_config_h
#define user_config_h

// Possible to set custom password here instead of UI
// #define ADMIN_PASSWORD "mega_password"

// uncomment here to apply wifi settings and to disable WiFiManager initial portal
// #define ESPWifiManualSetup true

#ifdef ESPWifiManualSetup
#  define wifi_ssid "exampleSSID"
#  define wifi_password "example_password"
#endif

#endif
