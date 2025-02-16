#!/bin/bash

VERSION="v1.0.0"

for file in .pio/build/*/firmware.bin; do
  env=$(echo "$file" | cut -f3 -d'/')
  cp "$file" "dist/mqtt433gateway_${env}-${VERSION}.bin"
done
