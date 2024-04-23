#include <iostream>

#include "rpc_framework_manager.h"
#include "EventLoop.h"
#include "InetAddress"

void InitServer()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(kServerPortInfo, grpc::InsecureServerCredentials());

    // 注册服务
    monitor::GrpcManagerImpl grpc_server;
    builder.RegisterService(&grpc_server);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server->Wait();

    EventLoop loop;
    InetAddress listenAddr(9981);
    monitor::rpcFrameworkManagerImpl impl;
    RpcServer server(&loop, listenAddr);
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