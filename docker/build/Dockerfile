FROM ubuntu:20.04

# 禁止交互式提示
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Shanghai

# 设置默认的shell位置
SHELL ["/bin/bash", "-c"]

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
    gdb \
    git \
    openssh-server \
    build-essential \
    libboost-all-dev \
    net-tools \
    vim \
    stress 

RUN apt-get install -y libc-ares-dev  libssl-dev gcc g++ make 
# RUN apt-get install -y  \
#     libx11-xcb1 \
#     libfreetype6 \
#     libdbus-1-3 \
#     libfontconfig1 \
#     libxkbcommon0   \
#     libxkbcommon-x11-0

# RUN apt-get install -y python-dev \
#     python3-dev \
#     python3-pip

# 复制运行依赖安装脚本
COPY install/protobuf /tmp/install/protobuf
RUN /tmp/install/protobuf/install_protobuf.sh

COPY install/abseil /tmp/install/abseil
RUN /tmp/install/abseil/install_abseil.sh

COPY install/grpc /tmp/install/grpc
RUN /tmp/install/grpc/install_grpc.sh

COPY install/cmake /tmp/install/cmake
RUN /tmp/install/cmake/install_cmake.sh

COPY install/gtest /tmp/install/gtest
RUN /tmp/install/gtest/install_gtest.sh

# RUN apt-get install -y python3-pip
# RUN pip3 install cuteci -i https://mirrors.aliyun.com/pypi/simple

# COPY install/qt /tmp/install/qt
# RUN /tmp/install/qt/install_qt.sh
