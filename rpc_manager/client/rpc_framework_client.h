#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
// #include <grpc/grpc.h>
// #include <grpcpp/create_channel.h>
// #include <grpcpp/grpcpp.h>

#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpClient.h"
#include "TcpConnection.h"
#include "RpcChannel.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "log_manager.h"

namespace monitor
{
    class RpcClient
    {
    public:
        RpcClient(EventLoop *loop, const InetAddress &serverAddr);
        RpcClient(EventLoop *loop);
        ~RpcClient();
        void SetMonitorInfo(const monitor::proto::MonitorInfo &monito_info);
        void GetMonitorInfo(monitor::proto::MonitorInfo *monito_info);
        void Connect() { client_.connect(); }

    private:
        std::unique_ptr<monitor::proto::GrpcManager::Stub> stubPtr_;
        EventLoop *loop_;
        TcpClient client_;
        RpcChannelPtr channel_;
    };
}
