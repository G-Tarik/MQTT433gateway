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

const char PROGMEM SETTINGS_FILE[] = "/settings.json";

const char PROGMEM DEFAULT_NAME[] = "rf434";
const char PROGMEM DEFAULT_PASSWORD[] = "MQTT433gateway";
const char PROGMEM DEFAULT_RECEIVE_TOPIC_SUFFIX[] = "/recv/";
const char PROGMEM DEFAULT_SEND_TOPIC_SUFFIX[] = "/send/";
const char PROGMEM DEFAULT_STATE_TOPIC_SUFFIX[] = "/state";
const char PROGMEM DEFAULT_VERSION_TOPIC_SUFFIX[] = "/version";
const char PROGMEM DEFAULT_RF_PROTOCOLS[] = "[]";
const char PROGMEM DEFAULT_SERIAL_LOG_LEVEL[] = "debug";
const char PROGMEM DEFAULT_WEB_LOG_LEVEL[] = "info";

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

const size_t SETTINGS_JSON_DOC_SIZE = 4096;

class Settings {
 public:
  using SettingTypeSet = std::bitset<SettingType::_END>;
  using SettingCallbackFn = std::function<void(const Settings &)>;

  Settings()
      : deviceName(FPSTR(DEFAULT_NAME)),
        configPassword(FPSTR(DEFAULT_PASSWORD)),
        mqttBroker(""),
        mqttBrokerPort(1883),
        mqttUser(""),
        mqttPassword(""),
        mqttRetain(false),
        mqttReceiveTopic(deviceName + FPSTR(DEFAULT_RECEIVE_TOPIC_SUFFIX)),
        mqttSendTopic(deviceName + FPSTR(DEFAULT_SEND_TOPIC_SUFFIX)),
        mqttStateTopic(deviceName + FPSTR(DEFAULT_STATE_TOPIC_SUFFIX)),
        mqttVersionTopic(deviceName + FPSTR(DEFAULT_VERSION_TOPIC_SUFFIX)),
        rfEchoMessages(false),
        rfReceiverPin(4),  // avoid 0, 2, 15, 16
        rfTransmitterPin(12),
        rfReceiverPinPullUp(false),
        rfProtocols(FPSTR(DEFAULT_RF_PROTOCOLS)),
        rfRawMinLength(64),
        serialLogLevel(FPSTR(DEFAULT_SERIAL_LOG_LEVEL)),
        webLogLevel(FPSTR(DEFAULT_WEB_LOG_LEVEL)),
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
    DynamicJsonDocument jsonDoc(SETTINGS_JSON_DOC_SIZE);
    doSerialize(jsonDoc, sensible);
    if (pretty) {
      serializeJsonPretty(jsonDoc, target);
    } else {
      serializeJson(jsonDoc, target);
    }
  }
  void deserialize(const String &json);
  void reset();
  bool hasValidPassword() const;
  void registerChangeHandler(SettingType setting,
                             const SettingCallbackFn &callback);

  String deviceName;
  String configPassword;
  String mqttBroker;
  uint16_t mqttBrokerPort;
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
  int8_t rfRawMinLength;
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
