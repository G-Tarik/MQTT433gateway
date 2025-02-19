#!/usr/bin/env python
import os
from subprocess import check_output, CalledProcessError
import json
import platformio
from platformio.package.manager.library import LibraryPackageManager
from platformio.package.manager.platform import PlatformPackageManager

# Try to import `env` - PlatformIO's black magic
try:
    Import("env")
except NameError as e:
    env = None
    print(e)

OUTFILE = "lib/version/version_build.h"
TEMPLATE = """static const char PROGMEM fw_version[] = "{version:s}";
static const char PROGMEM fw_build_with[] = "{build_with:s}";
"""
CHARS_TO_ESCAPE = ('\\', '"')


def get_fw_version():
    try:
        return check_output(['git', 'describe', '--abbrev=8', '--dirty', '--always', '--tags']).strip().decode()
    except (CalledProcessError, OSError):
        return "unknown"


def get_dependencies(env):
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

    return json.dumps(dependencies, separators=(',', ':'), sort_keys=True)


def escape_string(text, escape_chars=CHARS_TO_ESCAPE):
    for char in escape_chars:
        text = text.replace(char, '\\' + char)
    return text


def pre_action(env):
    fw_ver = escape_string(get_fw_version())
    dep_ver = escape_string(get_dependencies(env))
    with open(OUTFILE, 'w') as f:
        f.write(TEMPLATE.format(version=fw_ver, build_with=dep_ver))

    print(f"PRESCRIPT: Created file {OUTFILE} with fw_version {fw_ver}")

    if os.environ.get("PIO_BUILD_WEB") == "yes":
        print("PRESCRIPT: (re)building web ...")
        check_output(["scripts/build_web.sh"])
    else:
        print("PRESCRIPT: skipping (re)building web")


if not env.IsCleanTarget():
    pre_action(env)
