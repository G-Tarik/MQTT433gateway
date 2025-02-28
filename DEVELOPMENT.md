# PlatformIO extension for Vscode but without built-in PlatformIO Core

Using already exiting PlatformIO Core. Despite all the correct extension settings it still checks for built-in PIO Core on the start (this is clearly a bug).
To make IDE work close vscode, remove directory `.vscode` in the current project then run
```bash
pio pkg update
pio init --ide vscode
```
Identify this line https://github.com/platformio/platformio-vscode-ide/blob/3af388134660f5c347be18686cbd349cca91c235/src/main.js#L64  in the
~/.vscode/extensions/platformio.platformio-ide-3.3.4-linux-x64/dist/extension.js  and remove `await this.startInstaller(!hasPIOProject);`

# Build
This fork will not maintain Windows support. The scripts and commands are provided only for Linux OS without details for any specific distros.

Web frontend usually does not change often but if it is needed to rebuild then do it with (Docker is required):

```bash
pio run --target clean
PIO_BUILD_WEB=yes pio run
```

or build web separately before running `pio run` with the script :
```bash
./scripts/build_web.sh
```

# Debug
`pio device monitor -f esp8266_exception_decoder --port /dev/ttyUSB0`
