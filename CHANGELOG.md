# Changelog


## v2025.3.4
- Make settings easier to configure on build time
- Remove Mdns feature to save memory. High RAM usage is a bottleneck for MQTT TLS feature
- Implementation of MQTT TLS. Known issues:
    - Letsencrypt root CA cert is precompiled
    - sometimes too high RAM usage causes an OOM crash when web UI is used
- Add `LED_HEARTBEAT_INTERVAL` build flag to set custom LED blinking interval

## v2025.02.14

#### Fork created

- Fix deprecated `platformio.managers`
- Fix compilation by adding `extern`
- Add source code of latest ESPiLight and Pilight
- Improve scripts
- Send raw signals also to MQTT, not only to logs
- Make possible to drop raw signals which are shorter then user specified min length (configurable via UI)
- Add `User_config.h` to be able to build with custom predefined settings
