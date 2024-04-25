## 介绍
1、Docker模块：用dockerfile指定CMake、gRPC、Protocol Buffers等多个依赖，在容器中构建整个项目环境，以便在多台服务器上部署环境。  
2、Monitor模块：实现灵活的监控接口，能够有效地捕获CPU状态、系统负载、软中断、内存、网络等性能指标；为了模拟真实的性能问题，使用stress工具进行模拟压测，分析相应时刻服务器的cpu状况和终端状况。  
3、调用gRPC框架：构建Server和Client，实现模块之间的远程连接，同时降低模块间的耦合性。  
4、Protocol Buffers序列化：构建整个项目的数据结构，并确保高效的数据交换。 


## docker部分
便于安装相关依赖环境  

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


## 日志系统
### 阻塞队列
用于并行状态下的任务处理, 用了互斥锁  

### buffer
重写了buffer, 让缓冲区管理更加高效  

### log
异步日志  

## 压测
### CPU测试（开启3个CPU进程执行sqrt计算）
```shell
stress --cpu 3 --timeout 60
```
> 通过cpustat的cpu_percent数值趋近于76.56%，与user（用户态）使用率接近75.73%。
> 同时cpuload中数值也逐渐上升，负载增加 

### IO测试（开启4个IO进程，执行sync系统调用，刷新内存缓冲区到磁盘）
```shell
stress --io 4 --timeout 60
```
> cpu_percent、system、io_wait列数值逐渐上升，user数值降低 

排查步骤
①当 user 占用率过高的时候，通常是某些个别的进程占用了大量的 CPU，这时候很容易通过 top 找到该程序；此时如果怀疑程序异常，可以通过 perf 等思路找出热点调用函数来进一步排查；
②当 system 占用率过高的时候，如果 IO 操作(包括终端 IO)比较多，可能会造成这部分的 CPU 占用率高，比如在 file server、database server 等类型的服务器上，否则(比如>20%)很可能有些部分的内核、驱动模块有问题；
③当 nice 占用率过高的时候，通常是有意行为，当进程的发起者知道某些进程占用较高的 CPU，会设置其 nice 值确保不会淹没其他进程对 CPU 的使用请求；
④当 iowait 占用率过高的时候，通常意味着某些程序的 IO 操作效率很低，或者 IO 对应设备的性能很低以至于读写操作需要很长的时间来完成；
⑤当 irq/softirq 占用率过高的时候，很可能某些外设出现问题，导致产生大量的irq请求，这时候通过检查 /proc/interrupts 文件来深究问题所在；


## debug
cmake .. -DCMAKE_BUILD_TYPE=Debug && make   

## todo
- [ ] 异常处理   
- [ ] ai调参调优  
- [ ] 假如有100台, 怎么集中管理  
- [ ] 任务三怎么实现  
- [ ] 负载均衡   
- [x] 自己写一个rpc    
- [ ] 加数据库  