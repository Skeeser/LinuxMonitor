#pragma once

#include "network/TcpServer.h"

namespace google
{
    namespace protobuf
    {
        class Service;
    }
}

namespace network
{
    class RpcServer
    {
    public:
        RpcServer(EventLoop *loop, const InetAddress &listenAddr);

        void setThreadNum(int numThreads) { server_.setThreadNum(numThreads); }

        void registerService(::google::protobuf::Service *service);
        void start();

    private:
        void onConnection_(const TcpConnectionPtr &conn);

    private:
        TcpServer server_;
        std::map<std::string, ::google::protobuf::Service *> services_;
    };
}
