#pragma once
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpClient.h"
#include "TcpConnection.h"
#include "rpc_channel.h"
#include "rf_monitor_info.pb.h"
#include "log_manager.h"

namespace monitor
{
    class RpcClient
    {
    public:
        RpcClient(network::EventLoop *loop, const network::InetAddress &serverAddr);
        RpcClient(network::EventLoop *loop);
        ~RpcClient();
        void SetMonitorInfo(const monitor::proto::MonitorInfo &monito_info);
        void GetMonitorInfo(monitor::proto::MonitorInfo *monito_info);
        void connect() { client_.connect(); }

    private:
        void onConnection_(const network::TcpConnectionPtr &conn);
        void closure_(::monitor::proto::MonitorInfo *resp);

    private:
        monitor::proto::RpcManager::Stub stub_;
        network::EventLoop *loop_;
        network::TcpClient client_;
        network::RpcChannelPtr channel_;
    };
}
