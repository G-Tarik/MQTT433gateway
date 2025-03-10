#ifndef user_config_h
#define user_config_h

#define STRINGIFY(...) #__VA_ARGS__
#define TOSTRING(x) STRINGIFY(x)

/*
These parameters can be defined as build flags in user_extras.ini
*/

// uncomment or specify as build flags to apply custom wifi settings
// if these are not defined then default WiFiManager portal will start with ADMIN_PASSWORD
// #define WIFI_SSID exampleSSID
// #define WIFI_PASSWORD example_password


//#define WIFI_SSID "exampleSSID"
//#define WIFI_PASSWORD "example_password"

#ifndef DEVICE_NAME
#define DEVICE_NAME rf434
#endif

#ifndef ADMIN_PASSWORD
#define ADMIN_PASSWORD MQTT433gateway
#endif

#ifndef RF_PROTOCOLS
#define RF_PROTOCOLS []
#endif

#ifndef RF_RAW_MIN_LEN
#define RF_RAW_MIN_LEN 64
#endif

#ifndef RF_RX_PIN
#define RF_RX_PIN 4
#endif

#ifndef RF_TX_PIN
#define RF_TX_PIN 12
#endif

#ifndef SERIAL_LOG_LEVEL
#define SERIAL_LOG_LEVEL debug
#endif

#ifndef WEB_LOG_LEVEL
#define WEB_LOG_LEVEL info
#endif

#ifndef MQTT_RECEIVE_TOPIC
#define MQTT_RECEIVE_TOPIC /rf/recv/
#endif

#ifndef MQTT_SEND_TOPIC
#define MQTT_SEND_TOPIC /rf/send/
#endif

#ifndef MQTT_STATE_TOPIC
#define MQTT_STATE_TOPIC /rf/state
#endif

#ifndef MQTT_VERSION_TOPIC
#define MQTT_VERSION_TOPIC /rf/version
#endif

#ifndef MQTT_HOST
#define MQTT_HOST
#endif

#ifndef MQTT_PORT
#define MQTT_PORT 1883
#endif

#ifndef MQTT_ENABLE_TLS
#define MQTT_ENABLE_TLS false
#endif

#ifndef MQTT_USERNAME
#define MQTT_USERNAME
#endif

#ifndef MQTT_PASSWORD
#define MQTT_PASSWORD
#endif

#ifndef MQTT_RETAIN
#define MQTT_RETAIN false
#endif

#endif // user_config.h
