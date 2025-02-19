#!/bin/bash

VERSION=$(git describe --abbrev=8 --dirty --always --tags)

for file in .pio/build/*/firmware.bin; do
  env=$(echo "$file" | cut -f3 -d'/')
  cp -v "$file" "dist/mqtt433gateway_${env}-${VERSION}.bin"
done
