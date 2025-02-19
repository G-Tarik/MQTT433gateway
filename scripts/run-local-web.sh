#!/bin/bash

gunzip web/build/index.html.gz &&
docker run --rm -d --name frontweb -v $(pwd)/web/build/index.html:/usr/share/caddy/index.html -p 8080:80 caddy:alpine &&
gio open http://localhost:8080 &>/dev/null
