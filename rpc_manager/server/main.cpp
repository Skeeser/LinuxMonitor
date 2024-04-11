#include <iostream>
#include <grpc/grpc.h>
#include <grpc/server_builder.h>
#include "rpc_manager.h"

constexpr char kServerPortInfo[] = "0.0.0.0:50051";
void InitServer(){
    grpc::server_builder builder;
    builder.AddListeningPort(kServerPortInfo, grpc::InsecureServerCredentials());

    // 注册服务

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    server.Wait();
    return;
}


int main(){
    InitServer();
    return 0;
}