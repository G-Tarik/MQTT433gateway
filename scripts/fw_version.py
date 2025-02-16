"""MQTT433gateway git firmware version extractor

Project home: https://github.com/puuu/MQTT433gateway/
"""
import subprocess
import json
import platformio
from platformio.package.manager.library import LibraryPackageManager
from platformio.package.manager.platform import PlatformPackageManager
Import("env")

OUTFILE = "dist/version_build.h"
TEMPLATE = """
static const char PROGMEM fw_version[] = "{version:s}";
static const char PROGMEM fw_build_with[] = "{build_with:s}";
"""
CHARS_TO_ESCAPE = ('\\', '"')


def get_fw_version():
    try:
        return subprocess.check_output(['git', 'describe', '--abbrev=8', '--dirty',
                                        '--always', '--tags']).strip().decode()
    except (subprocess.CalledProcessError, OSError):
        return "unknown"


def get_dependencies():
    lib_mgr = LibraryPackageManager()
    platform_mgr = PlatformPackageManager()

    # Get installed libraries
    dependencies = {
        pkg.metadata.name: pkg.metadata.version
        for pkg in lib_mgr.get_installed()
    }

    # Get platform information
    platform = env['PIOPLATFORM']
    dependencies[platform] = str(platform_mgr.get_installed()[0].metadata.version)
    dependencies['PlatformIO'] = platformio.__version__

    return dependencies


def get_dependencies_json():
    return json.dumps(get_dependencies(), separators=(',', ':'), sort_keys=True)


def escape_string(text, escape_chars=CHARS_TO_ESCAPE):
    for char in escape_chars:
        text = text.replace(char, '\\' + char)
    return text


def generate_file():
    with open(OUTFILE, 'w') as f:
        f.write(TEMPLATE.format(version=escape_string(get_fw_version()),
                                build_with=escape_string(get_dependencies_json())))

    subprocess.check_output(["scripts/release.sh"])


generate_file()
