#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"
#include "log_manager.h"

namespace monitor
{
    class RpcClient
    {
    public:
        RpcClient();
        ~RpcClient();
        void SetMonitorInfo(const monitor::proto::MonitorInfo &monito_info);
        void GetMonitorInfo(monitor::proto::MonitorInfo *monito_info);

    private:
        std::unique_ptr<monitor::proto::GrpcManager::Stub> stubPtr_;
    };
}
