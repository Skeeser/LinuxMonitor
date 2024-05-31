<div align="center">

<img alt="LOGO" src="assets/log.png" height="200" />

# LinuxMonitor

<br>

<div>
    <img alt="c++" src="https://img.shields.io/badge/c++-11-%2300599C">
</div>
<div>
    <img alt="platform" src="https://img.shields.io/badge/platform-Linux%20-blueviolet">
</div>
<div>
    <img alt="license" src="https://img.shields.io/github/license/Skeeser/LinuxMonitor">
    <img alt="commit" src="https://img.shields.io/github/commit-activity/m/Skeeser/LinuxMonitor?color=%23ff69b4">
    <img alt="stars" src="https://img.shields.io/github/stars/Skeeser/LinuxMonitor?style=social">
</div>
<br>

[简体中文](README_ZH.md)  | [English](README_EN.md) 

本系统采用C++技术栈，并结合 Docker、gRPC、Protocol Buffers等工具和框架的分布式Linux性能分析监控系统。通过模块化的设计，该项目旨在实现对分布式Linux 系统的性能监控和分析。 <br>

</div>

<br>

## 功能特性
1、Docker模块：用dockerfile指定CMake、gRPC、Protocol Buffers等多个依赖，在容器中构建整个项目环境，以便在多台服务器上部署环境。  
2、Monitor模块：实现灵活的监控接口，能够有效地捕获CPU状态、系统负载、软中断、内存、网络等性能指标；为了模拟真实的性能问题，使用stress工具进行模拟压测，分析相应时刻服务器的cpu状况和终端状况。  
3、调用gRPC框架：构建Server和Client，实现模块之间的远程连接，同时降低模块间的耦合性。  
4、Protocol Buffers序列化：构建整个项目的数据结构，并确保高效的数据交换。  
5、实现异步/同步的高性能日志系统，可以记录程序运行的状态。 

<br>

## 文件结构
./  
├── assets => 资源文件   
├── doc => 存放开发相关文档   
├── docker   => docker镜像相关   
├── log_manager => 日志系统  
├── monitor_work => 监控功能代码  
├── proto => 定义protobuf  
└── rpc_manager => grpc相关  


<br>


## 系统架构
Linux  

<br>


## 应用技术
C++、Docker、gRPC、protobuf、Cmake、异步日志

<br>

## 构建
### 先构建docker容器
#### 镜像创建 
```shell
cd docker/scripts
./docker_build.sh
```

#### 创建容器
```shell
./docker_run.sh
```

#### 进入容器
```shell
./docker_into.sh
```

#### 删除容器
```shell
./docker_stop_and_run.sh
```

<br>

### 然后编译
进入容器后
```shell
cd work
mkdir build
cd build
cmake ..
make -j8
```

<br>

## 运行
### 启动grpc服务(主服务器)
在`build`文件夹下  
```shell
./rpc_manager/server/server
```

### 启动监控(需要监控的服务器)
```shell
./monitor_work/src/monitor
```

### 展示数据
- [ ] 前端待做

<br>

## 使用指南

<!-- 描述如何使用该项目 -->
<br>

## 如何贡献
如果你碰巧看见这个项目, 想要参与开发  
可以查看这个文档 [如何参与开源项目](doc/github参与开源项目流程.md)  

如果想了解这个项目, 可以查看[开发文档](doc/dev.md)  

<br>

## 许可证
GPL  
