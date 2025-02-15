#!/usr/bin/env python

"""MQTT433gateway web interface builder

Project home: https://github.com/puuu/MQTT433gateway/
"""
import os
from subprocess import check_output


if os.environ.get("PIO_BUILD_WEB") == "yes":
    print("INFO: (re)building web ...")
    check_output(["scripts/build_web.sh"])
else:
    print("INFO: skipping (re)building web")
