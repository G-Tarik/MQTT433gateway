#!/bin/bash

gunzip build/index.html.gz
docker run --rm -d --name frontweb -v $(pwd)/build/index.html:/usr/share/caddy/index.html -p 8080:80 caddy:alpine

