#!/usr/bin/env bash

echo "stop and rm docker container" 
docker stop monitor_container > /dev/null
docker rm -v -f monitor_container > /dev/null