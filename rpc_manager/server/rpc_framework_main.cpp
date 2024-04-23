#include <iostream>

#include "rpc_framework_manager.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "rpc_server.h"

void InitServer()
{
    network::EventLoop loop;
    // 固定端口为50051
    network::InetAddress listenAddr(50051);
    monitor::rpcFrameworkManagerImpl impl;
    network::RpcServer server(&loop, listenAddr);
    server.registerService(&impl);
    server.start();
    loop.loop();
    google::protobuf::ShutdownProtobufLibrary();
    return;
}

int main()
{
    InitServer();
    return 0;
}