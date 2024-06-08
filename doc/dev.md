# 开发部分
## docker
### dockerfile编写
install依赖介绍
- protobuf  
见下
- abseil
Google 开源的其内部多年使用的 C++ 代码库 Abseil , 作为 C++ 标准库的补充  


<br>

## protobuf
### 介绍
#### 大概介绍
Protobuf（Protocol Buffers）是由 Google 开发的一种轻量级、高效的数据交换格式，它被用于结构化数据的序列化、反序列化和传输。相比于 XML 和 JSON 等文本格式，Protobuf 具有更小的数据体积、更快的解析速度和更强的可扩展性。 

Protobuf 的核心思想是使用协议（Protocol）来定义数据的结构和编码方式。使用 Protobuf，可以先定义数据的结构和各字段的类型、字段等信息，然后使用 Protobuf 提供的编译器生成对应的代码，用于序列化和反序列化数据。由于 Protobuf 是基于二进制编码的，因此可以在数据传输和存储中实现更高效的数据交换，同时也可以跨语言使用。 

<br>

#### 优势

相比于 XML 和 JSON，Protobuf 有以下几个优势： 
更小的数据量：Protobuf 的二进制编码通常只有 XML 和 JSON 的 1/3 到 1/10 左右，因此在网络传输和存储数据时可以节省带宽和存储空间。 
更快的序列化和反序列化速度：由于 Protobuf 使用二进制格式，所以序列化和反序列化速度比 XML 和 JSON 快得多。 
跨语言：Protobuf 支持多种编程语言，可以使用不同的编程语言来编写客户端和服务端。这种跨语言的特性使得 Protobuf 受到很多开发者的欢迎（JSON 也是如此）。 
易于维护可扩展：Protobuf 使用 .proto 文件定义数据模型和数据格式，这种文件比 XML 和 JSON 更容易阅读和维护，且可以在不破坏原有协议的基础上，轻松添加或删除字段，实现版本升级和兼容性。 

<br>

### 定义
cpu, net, mem的信息  
还有相关service 

<br>


## rpc
### grpc介绍
gRPC 是一个高性能、开源、通用的RPC框架，由Google推出，基于HTTP2协议标准设计开发，默认采用Protocol Buffers数据序列化协议，支持多种开发语言。gRPC提供了一种简单的方法来精确的定义服务，并且为客户端和服务端自动生成可靠的功能库。


<br>

## 日志系统
### 阻塞队列
用于并行状态下的任务处理, 用了互斥锁  

<br>

### buffer
重写了buffer, 让缓冲区管理更加高效  

<br>

### log
异步日志  


<br>


# QandA
## 如何添加新的要监测的数据文件?
### 添加数据结构
在`proto`文件夹下定义新的proto文件, 并且在`monitor_info.proto`的`message MonitorInfo`添加对应的消息  

<br>

### 添加读取文件的程序
在`monitor_work/`下写, 照着写就行了, 继承`MonitorInter`的接口, 实现`UpdateOnce`函数  

<br>

### 添加监控程序, 也就是client对新的数据的调用  
在`LinuxMonitor/monitor_work/src/main.cpp`中   
```c++
runners.emplace_back(new monitor::CpuSoftIrqMonitor());
runners.emplace_back(new monitor::CpuLoadMonitor());
runners.emplace_back(new monitor::CpuStatMonitor());
runners.emplace_back(new monitor::MemMonitor());
runners.emplace_back(new monitor::NetMonitor());
```
下面加进runners中  

<br>

## server端是怎么调用的?
### rpc_manager
在`LinuxMonitor/rpc_manager/server/rpc_manager.cpp`中  
有一行  
```c++
// 通过日志写入
LOG_INFO("monitor_info:\n%s", request->DebugString().c_str());
```
这就是把request里面的数据存到日志里了,以及类内部的`monitorInfos_`对象   
而`SetMonitorInfo`是我们在client里调用的  

<br>

### 怎么用数据
前端或者数据库程序直接调用`GetMonitorInfo`就可以获取到数据了  

<br>

# todo
异常处理
ai调参调优
假如有100台, 怎么集中管理
任务三怎么实现