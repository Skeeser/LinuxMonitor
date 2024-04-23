#pragma once

// #include <grpcpp/support/status.h>
#include <unordered_map>

#include "monitor_info.pb.h"
#include "log_manager.h"
#include "EventLoop.h"
#include "rpc_server.h"

// 写rpc manager的服务代码
namespace monitor
{
    // 继承服务类
    class rpcFrameworkManagerImpl : public monitor::proto::GrpcManager::Service
    {
    public:
        rpcFrameworkManagerImpl();
        virtual ~rpcFrameworkManagerImpl();

        // 返回全局命名空间的grpc的状态
        void SetMonitorInfo(::google::protobuf::RpcController *controller,
                            const ::monitor::proto::MonitorInfo *request,
                            ::google::protobuf::Empty *response,
                            ::google::protobuf::Closure *done);
        void GetMonitorInfo(::google::protobuf::RpcController *controller,
                            const ::google::protobuf::Empty *request,
                            ::monitor::proto::MonitorInfo *response,
                            ::google::protobuf::Closure *done);

    private:
        monitor::proto::MonitorInfo monitorInfos_;
    };

}