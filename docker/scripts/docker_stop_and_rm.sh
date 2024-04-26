#!/usr/bin/env bash

# 删除容器

echo "stop and rm docker" 
docker stop monitor_container > /dev/null
docker rm -v -f monitor_container > /dev/null