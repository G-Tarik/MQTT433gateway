/**
  MQTT433gateway - MQTT 433.92 MHz radio gateway utilizing ESPiLight
  Project home: https://github.com/puuu/MQTT433gateway/

  The MIT License (MIT)

  Copyright (c) 2017 Jan Losinski

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef MQTT433GATEWAY_SETTINGS_H
#define MQTT433GATEWAY_SETTINGS_H

#include <bitset>
#include <forward_list>
#include <functional>

#include <Stream.h>
#include <WString.h>

#include <ArduinoJson.h>

#include "../../src/User_config.h"

const char PROGMEM SETTINGS_FILE[] = "/settings.json";

enum SettingType {
  BASE,
  WEB_CONFIG,
  MQTT,
  RF_ECHO,
  RF_CONFIG,
  RF_PROTOCOL,
  LOGGING,
  SYSLOG,
  STATUSLED,
  _END
};

const size_t SETTINGS_JSON_DOC_SIZE = 2048;

class Settings {
 public:
  using SettingTypeSet = std::bitset<SettingType::_END>;
  using SettingCallbackFn = std::function<void(const Settings &)>;

  Settings()
      : deviceName(TOSTRING(DEVICE_NAME)),
        configPassword(TOSTRING(ADMIN_PASSWORD)),
        mqttBroker(TOSTRING(MQTT_HOST)),
        mqttBrokerPort(MQTT_PORT),
        mqttTls(MQTT_ENABLE_TLS),
        mqttUser(TOSTRING(MQTT_USERNAME)),
        mqttPassword(TOSTRING(MQTT_PASSWORD)),
        mqttRetain(MQTT_RETAIN),
        mqttReceiveTopic(TOSTRING(MQTT_RECEIVE_TOPIC)),
        mqttSendTopic(TOSTRING(MQTT_SEND_TOPIC)),
        mqttStateTopic(TOSTRING(MQTT_STATE_TOPIC)),
        mqttVersionTopic(TOSTRING(MQTT_VERSION_TOPIC)),
        rfEchoMessages(false),
        rfReceiverPin(RF_RX_PIN),  // avoid 0, 2, 15, 16 // why?
        rfTransmitterPin(RF_TX_PIN),
        rfReceiverPinPullUp(false),
        rfProtocols(TOSTRING(RF_PROTOCOLS)),
        rfRawMinLength(RF_RAW_MIN_LEN),
        serialLogLevel(TOSTRING(SERIAL_LOG_LEVEL)),
        webLogLevel(TOSTRING(WEB_LOG_LEVEL)),
        syslogLevel(""),
        syslogHost(""),
        syslogPort(514),
        ledPin(LED_BUILTIN),
        ledActiveHigh(false) {}
  ~Settings();
  void load();
  void save();
  void notifyAll();
  template <typename T>
  void serialize(T &target, bool pretty, bool sensible = true) const {
    // DynamicJsonDocument jsonDoc(SETTINGS_JSON_DOC_SIZE);
    JsonDocument jsonDoc;
    doSerialize(jsonDoc, sensible);
    if (pretty) {
      serializeJsonPretty(jsonDoc, target);
    } else {
      serializeJson(jsonDoc, target);
    }
  }
  void deserialize(const String &json);
  void reset();
  void registerChangeHandler(SettingType setting,
                             const SettingCallbackFn &callback);

  String deviceName;
  String configPassword;
  String mqttBroker;
  uint16_t mqttBrokerPort;
  bool mqttTls;
  String mqttUser;
  String mqttPassword;
  bool mqttRetain;
  String mqttReceiveTopic;
  String mqttSendTopic;
  String mqttStateTopic;
  String mqttVersionTopic;
  bool rfEchoMessages;
  int8_t rfReceiverPin;
  int8_t rfTransmitterPin;
  bool rfReceiverPinPullUp;
  String rfProtocols;
  uint8_t rfRawMinLength;
  String serialLogLevel;
  String webLogLevel;
  String syslogLevel;
  String syslogHost;
  uint16_t syslogPort;
  uint8_t ledPin;
  bool ledActiveHigh;

 private:
  struct SettingListener {
    const SettingType type;
    const Settings::SettingCallbackFn callback;

    SettingListener(const SettingType &type,
                    const Settings::SettingCallbackFn &cb)
        : type(type), callback(cb) {}
  };

  void onConfigChange(SettingTypeSet typeSet) const;
  SettingTypeSet applyJson(JsonDocument &parsedSettings);
  void doSerialize(JsonDocument &jsonDoc, bool sensible = true) const;

  std::forward_list<SettingListener> listeners;
};

#endif  // MQTT433GATEWAY_SETTINGS_H
