#!/bin/bash

if [[ -z "$1" ]]; then
  echo "VERSION is missing"
  exit 1
fi

VERSION="$1"

for file in .pio/build/*/firmware.bin; do
  env=$(echo "$file" | cut -f3 -d'/')
  cp -v "$file" "dist/mqtt433gateway_${env}-${VERSION}.bin"
done
