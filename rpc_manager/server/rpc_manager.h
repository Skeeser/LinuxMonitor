#pragma once

#include <grpcpp/support/status.h>
#include <unordered_map>

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

// 写rpc manager的服务代码
namespace monitor{
class GrpcManagerImpl : public monitor::proto::GrpcManager::Service{
    public:
    GrpcManagerImpl();
    virtual ~GrpcManagerImpl();

    // 返回全局命名空间的grpc的状态
    ::grpc::Status SetMonitorInfo(::grpc::ServerContext* context,
                                const ::monitor::proto::MonitorInfo* request,
                                ::google::protobuf::Empty* response);
    ::grpc::Status GetMonitorInfo(::grpc::ServerContext* context,
                                const ::google::protobuf::Empty* request,
                                ::monitor::proto::MonitorInfo* response);

    private:
    monitor::proto::MonitorInfo monitor_infos_;

}


}