## 介绍
1、Docker模块：用dockerfile指定CMake、gRPC、Protocol Buffers等多个依赖，在容器中构建整个项目环境，以便在多台服务器上部署环境。  
2、Monitor模块：实现灵活的监控接口，能够有效地捕获CPU状态、系统负载、软中断、内存、网络等性能指标；为了模拟真实的性能问题，使用stress工具进行模拟压测，分析相应时刻服务器的cpu状况和终端状况。  
3、调用gRPC框架：构建Server和Client，实现模块之间的远程连接，同时降低模块间的耦合性。  
4、Protocol Buffers序列化：构建整个项目的数据结构，并确保高效的数据交换。 




## docker部分
### 镜像创建 
```shell
./docker_build.sh
```

### 运行镜像脚本
```shell
./docker_run.sh
./docker_into.sh
```

## 镜像内
### 编译
```shell
cd work/cmake/
cmake ..
make -j8
```

### 启动grpc服务
```shell
cd rpc_manager/server/
./server

```

### 启动监控
```shell
cd work/cmake/test_monitor/.monitor
```

### 展示数据
另开一个终端  
```shell
cd work/cmake/display/
./display
```

# 开发部分
## docker
### dockerfile编写
install依赖介绍
- protobuf  
见下
- abseil
Google 开源的其内部多年使用的 C++ 代码库 Abseil , 作为 C++ 标准库的补充  
- qt
要到对应网址进行下载, 1.3G, 放入对应文件夹  


## protobuf
### 介绍
#### 大概介绍
Protobuf（Protocol Buffers）是由 Google 开发的一种轻量级、高效的数据交换格式，它被用于结构化数据的序列化、反序列化和传输。相比于 XML 和 JSON 等文本格式，Protobuf 具有更小的数据体积、更快的解析速度和更强的可扩展性。 

Protobuf 的核心思想是使用协议（Protocol）来定义数据的结构和编码方式。使用 Protobuf，可以先定义数据的结构和各字段的类型、字段等信息，然后使用 Protobuf 提供的编译器生成对应的代码，用于序列化和反序列化数据。由于 Protobuf 是基于二进制编码的，因此可以在数据传输和存储中实现更高效的数据交换，同时也可以跨语言使用。 

#### 优势

相比于 XML 和 JSON，Protobuf 有以下几个优势： 
更小的数据量：Protobuf 的二进制编码通常只有 XML 和 JSON 的 1/3 到 1/10 左右，因此在网络传输和存储数据时可以节省带宽和存储空间。 
更快的序列化和反序列化速度：由于 Protobuf 使用二进制格式，所以序列化和反序列化速度比 XML 和 JSON 快得多。 
跨语言：Protobuf 支持多种编程语言，可以使用不同的编程语言来编写客户端和服务端。这种跨语言的特性使得 Protobuf 受到很多开发者的欢迎（JSON 也是如此）。 
易于维护可扩展：Protobuf 使用 .proto 文件定义数据模型和数据格式，这种文件比 XML 和 JSON 更容易阅读和维护，且可以在不破坏原有协议的基础上，轻松添加或删除字段，实现版本升级和兼容性。 

### 定义
cpu, net, mem的信息  
还有相关service 


## rpc
### grpc介绍
gRPC 是一个高性能、开源、通用的RPC框架，由Google推出，基于HTTP2协议标准设计开发，默认采用Protocol Buffers数据序列化协议，支持多种开发语言。gRPC提供了一种简单的方法来精确的定义服务，并且为客户端和服务端自动生成可靠的功能库。


## todo
异常处理
ai调参调优
假如有100台, 怎么集中管理
任务三怎么实现