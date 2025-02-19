#ifndef user_config_h
#define user_config_h

// set custom password on compile time
// #define ADMIN_PASSWORD "example_password"

// uncomment here to apply wifi settings and to disable WiFiManager initial portal
// #define ESPWifiManualSetup true

#ifdef ESPWifiManualSetup
#  define wifi_ssid "exampleSSID"
#  define wifi_password "example_password"
#endif


#endif
