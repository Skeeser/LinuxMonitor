# 依赖
## docker部分
### 镜像创建 
```shell
docker build -t 镜像名称 .
```

### 运行镜像脚本
```shell
./mo...run.sh
./ml...into.sh
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