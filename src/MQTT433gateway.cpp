/**
  MQTT433gateway - MQTT 433.92 MHz radio gateway utilizing ESPiLight
  Project home: https://github.com/puuu/MQTT433gateway/

  The MIT License (MIT)

  Copyright (c) 2016 Puuu

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

#include "User_config.h"

#include <ESP8266httpUpdate.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include <ArduinoSimpleLogging.h>
#include <WiFiManager.h>

#include <ConfigWebServer.h>
#include <MqttClient.h>
#include <RfHandler.h>
#include <Settings.h>
#include <StatusLED.h>
#include <SyslogLogTarget.h>
#include <SystemHeap.h>
#include <SystemLoad.h>

Settings settings;
ConfigWebServer *webServer = nullptr;
MqttClient *mqttClient = nullptr;
RfHandler *rf = nullptr;
SyslogLogTarget *syslogLog = nullptr;
StatusLED *statusLED = nullptr;
SystemLoad *systemLoad = nullptr;
SystemHeap *systemHeap = nullptr;

void setupMqtt(const Settings &) {
  if (mqttClient != nullptr) {
    delete mqttClient;
    mqttClient = nullptr;
    Logger.debug.println(F("MQTT instance removed."));
  }

  if (settings.mqttBroker.length() <= 0) {
    Logger.warning.println(F("No MQTT broker configured yet"));
    return;
  }

  mqttClient = new MqttClient(settings);
  mqttClient->registerRfDataHandler(
      [](const String &protocol, const String &data) {
        if (rf) rf->transmitCode(protocol, data);
      });
  mqttClient->begin();
  Logger.info.println(F("MQTT instance created."));
}

void setupRf(const Settings &) {
  if (rf) {
    delete rf;
    rf = nullptr;
    Logger.debug.println(F("Rf instance removed."));
  }

  rf = new RfHandler(settings);
  rf->registerReceiveHandler([](const String &protocol, const String &data) {
    if (mqttClient) {
      mqttClient->publishCode(protocol, data);
    }
  });
  rf->setEchoEnabled(settings.rfEchoMessages);
  rf->filterProtocols(settings.rfProtocols);
  rf->begin();
  Logger.info.println(F("RfHandler Instance created."));
}

void setupWebLog() {
  if (webServer) {
    if (settings.webLogLevel.length() > 0) {
      Logger.addHandler(Logger.stringToLevel(settings.webLogLevel),
                        webServer->logTarget());
    } else {
      Logger.removeHandler(webServer->logTarget());
    }
  }
}

void setupWebServer() {
  webServer = new ConfigWebServer(settings);

  webServer->registerSystemCommandHandler(F("restart"), []() {
    Logger.info.println(F("Restart device."));
    delay(100);
    ESP.restart();
  });
  webServer->registerSystemCommandHandler(F("reset_wifi"), []() {
    Logger.info.println(F("Reset wifi and restart device."));
    WiFi.disconnect(true);
    delay(100);
    ESP.restart();
  });
  webServer->registerSystemCommandHandler(F("reset_config"), []() {
    Logger.info.println(F("Reset configuration and restart device."));
    settings.reset();
    delay(100);
    ESP.restart();
  });
  webServer->registerProtocolProvider(RfHandler::availableProtocols);
  webServer->registerOtaHook([]() {
    Logger.debug.println(F("Prepare for ota update."));
    if (statusLED) statusLED->setState(StatusLED::ota);
    if (rf) {
      rf->filterProtocols(F("[]"));
      delete rf;
      rf = nullptr;
    }
    if (mqttClient) {
      delete mqttClient;
      mqttClient = nullptr;
    }
    WiFiUDP::stopAll();
  });
  webServer->registerDebugFlagHandler(
      F("protocolRaw"), []() { return rf && rf->isRawModeEnabled(); },
      [](bool state) {
        if (rf) rf->setRawMode(state);
      });
  webServer->registerDebugFlagHandler(
      F("systemLoad"), []() { return systemLoad != nullptr; },
      [](bool state) {
        if (state) {
          systemLoad = new SystemLoad(Logger.info);
        } else if (systemLoad) {
          delete systemLoad;
          systemLoad = nullptr;
        }
      });
  webServer->registerDebugFlagHandler(
      F("freeHeap"), []() { return systemHeap != nullptr; },
      [](bool state) {
        if (state) {
          systemHeap = new SystemHeap(Logger.info);
        } else if (systemHeap) {
          delete systemHeap;
          systemHeap = nullptr;
        }
      });

  webServer->begin();
  Logger.info.println(F("WebServer instance created."));

  setupWebLog();
}

void setupStatusLED(const Settings &s) {
  delete statusLED;
  statusLED = new StatusLED(s.ledPin, s.ledActiveHigh);
  Logger.debug.print("Change status LED config: pin=");
  Logger.debug.print(s.ledPin);
  Logger.debug.print(" activeHigh=");
  Logger.debug.println(s.ledActiveHigh);
}

void setupWifi() {
#ifdef WIFI_SSID

  Logger.debug.println(F("Current configuration:"));
  settings.serialize(Logger.debug, true, false);
  Logger.debug.println();

  WiFi.mode(WIFI_STA);
  WiFi.hostname(settings.deviceName);
  WiFi.begin(TOSTRING(WIFI_SSID), TOSTRING(WIFI_PASSWORD));

  Logger.info.print(F("Connecting to WiFi..."));
  Logger.info.println(TOSTRING(WIFI_SSID));

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 30) {  // 30 retries (~15s timeout)
    delay(500);
    Logger.info.print(F("."));
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Logger.info.println(F("\nConnected to WiFi."));
  } else {
    Logger.warning.println(F("\nFailed to connect, rebooting..."));
    ESP.restart();
  }

#else
  WiFiManager wifiManager;
  WiFi.hostname(settings.deviceName);
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.setAPCallback([](WiFiManager *) {
    Logger.info.println(F("Start wifimanager config portal."));
    if (statusLED) statusLED->setState(StatusLED::wifimanager);
  });

  // Restart after we had the portal running
  wifiManager.setSaveConfigCallback([]() {
    Logger.info.println(F("Wifi config changed. Restart."));
    delay(100);
    ESP.restart();
  });

  if (!wifiManager.autoConnect(settings.deviceName.c_str(),
                               settings.configPassword.c_str())) {
    Logger.warning.println(F("Try connecting again after reboot"));
    ESP.restart();
  }
#endif
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Logger.addHandler(Logger.DEBUG, Serial);
  if (!SPIFFS.begin()) {
    Logger.error.println(F("Initializing of SPIFFS failed!"));
  }

  settings.registerChangeHandler(STATUSLED, setupStatusLED);
  settings.registerChangeHandler(MQTT, setupMqtt);
  settings.registerChangeHandler(RF_ECHO, [](const Settings &s) {
    Logger.debug.println(F("Configure rfEchoMessages."));
    if (rf) {
      rf->setEchoEnabled(s.rfEchoMessages);
    } else {
      Logger.warning.println(F("No Rf instance available"));
    }
  });
  settings.registerChangeHandler(RF_PROTOCOL, [](const Settings &s) {
    Logger.debug.println(F("Apply changes to rfProtocols."));
    if (rf) {
      rf->filterProtocols(s.rfProtocols);
    } else {
      Logger.warning.println(F("No Rf instance available"));
    }
  });
  settings.registerChangeHandler(WEB_CONFIG, [](const Settings &s) {
    Logger.debug.println(F("Configure WebServer."));
    if (!webServer) {
      setupWebServer();
    }
  });
  settings.registerChangeHandler(SYSLOG, [](const Settings &s) {
    if (syslogLog) {
      Logger.removeHandler(*syslogLog);
      delete syslogLog;
      syslogLog = nullptr;
      Logger.debug.println(F("Syslog instance removed."));
    }
    if (s.syslogLevel.length() > 0 && s.syslogHost.length() > 0 &&
        s.syslogPort != 0) {
      syslogLog = new SyslogLogTarget();
      syslogLog->begin(s.deviceName, s.syslogHost, s.syslogPort);
      Logger.debug.println(F("Syslog instance created."));
      Logger.addHandler(Logger.stringToLevel(s.syslogLevel), *syslogLog);
    }
  });
  settings.registerChangeHandler(RF_CONFIG, setupRf);
  settings.registerChangeHandler(LOGGING, [](const Settings &s) {
    Logger.debug.println(F("Configure logging."));
    if (s.serialLogLevel.length() > 0) {
      Logger.addHandler(Logger.stringToLevel(settings.serialLogLevel), Serial);
    } else {
      Logger.removeHandler(Serial);
    }
    setupWebLog();
  });

  Logger.info.println(F("Load Settings..."));
  settings.load();

  setupStatusLED(settings);
  if (statusLED) statusLED->setState(StatusLED::wifiConnect);

  Serial.print(F("configPassword: "));
  Serial.println(settings.configPassword);
  setupWifi();

  // Notify all setting listeners
  settings.notifyAll();
  if (statusLED) statusLED->setState(StatusLED::startup);

  Logger.debug.println(F("Current configuration:"));
  settings.serialize(Logger.debug, true, false);
  Logger.debug.println();
  Logger.info.print(F("Listen on IP: "));
  Logger.info.println(WiFi.localIP());

  // temporary for debugging OOM
  systemHeap = new SystemHeap(Logger.info, 5000);
}

void loop() {
  if (rf && mqttClient && mqttClient->isConnected()) {
    if (statusLED) statusLED->setState(StatusLED::normalOperation);
  } else {
    if (statusLED) statusLED->setState(StatusLED::requireConfiguration);
  }
  if (statusLED) {
    statusLED->loop();
  }

  if (webServer) {
    webServer->loop();
  }

  if (mqttClient) {
    mqttClient->loop();
  }

  if (rf) {
    rf->loop();
  }

  if (systemLoad) {
    systemLoad->loop();
  }
  if (systemHeap) {
    systemHeap->loop();
  }

}
