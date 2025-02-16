#!/bin/bash

docker run --rm -i -v $PWD/web:/build -w /build --user 1000:1000 node:alpine npm install
cp web/build/index.html.gz.h dist/index.html.gz.h
