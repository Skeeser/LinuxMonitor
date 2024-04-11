#!/usr/bin/env bash

# xhost 是用来控制X server访问权限的。
xhost +local:root 1>/dev/null 2>&1
docker exec \
    -u root \
    -it monitor_container \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1