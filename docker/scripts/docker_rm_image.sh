#!/usr/bin/env bash

echo "stop and rm docker image"
docker rmi -f linux_monitor:latest > /dev/null 