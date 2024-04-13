#include "rpc_manager.h"

namespace monitor
{
    GrpcManagerImpl::GrpcManagerImpl()
    {
        // 初始化日志
        Log::init(1, "../log/server", ".log", 1024);
    }

    GrpcManagerImpl::~GrpcManagerImpl() {}

    ::grpc::Status SetMonitorInfo(::grpc::ServerContext *context,
                                  const ::monitor::proto::MonitorInfo *request,
                                  ::google::protobuf::Empty *response)
    {
        monitor_infos_.Clear();
        // 赋值
        monitor_infos_ = *request;
        // 通过日志写入
        // std::cout << "into" << request->soft_irq_size() << std::endl;
        LOG_INFO("Soft_irq_size: %d", request->soft_irq_size());
        return ::grpc::Status::OK;
    }

    ::grpc::Status GetMonitorInfo(::grpc::ServerContext *context,
                                  const ::google::protobuf::Empty *request,
                                  ::monitor::proto::MonitorInfo *response)
    {
        *response = monitor_infos_;
        return ::grpc::Status::OK;
    }

}
