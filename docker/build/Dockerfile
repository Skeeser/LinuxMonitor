FROM ubuntu:20.04

# 禁止交互式提示
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

# 设置默认的shell位置
SHELL ["./bin/shell", "-c"]

# 配置apt, 删除软件包和配置软件源
RUN apt-get clean && \
    apt-get autoclean

COPY apt/sources.list /etc/apt/


RUN apt-get update  && apt-get upgrade -y  && \
    apt-get install -y \
    htop \
    apt-utils \
    curl \
    cmake \
    git \
    openssh-server \
    build-essential \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    libboost-all-dev \
    net-tools \
    vim \
    stress 

RUN apt-get install -y libc-ares-dev  libssl-dev gcc g++ make 
RUN apt-get install -y  \
    libx11-xcb1 \
    libfreetype6 \
    libdbus-1-3 \
    libfontconfig1 \
    libxkbcommon0   \
    libxkbcommon-x11-0

RUN apt-get install -y python-dev \
    python3-dev \
    python-pip \
    python-all-dev 

# 复制运行依赖安装脚本
