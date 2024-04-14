#include "rpc_manager.h"

namespace monitor
{
    GrpcManagerImpl::GrpcManagerImpl()
    {
        // 初始化日志
        Log::Instance()->init(1, "./server_log", ".log", 1024);
        LOG_INFO("<-----------------SERVER---------------->");
    }

    GrpcManagerImpl::~GrpcManagerImpl() {}

    ::grpc::Status GrpcManagerImpl::SetMonitorInfo(::grpc::ServerContext *context,
                                                   const ::monitor::proto::MonitorInfo *request,
                                                   ::google::protobuf::Empty *response)
    {
        monitorInfos_.Clear();
        // 赋值
        monitorInfos_ = *request;
        // 通过日志写入
        // std::cout << "into" << request->soft_irq_size() << std::endl;
        LOG_INFO("Soft_irq_size: %d", request->soft_irq_size());
        return ::grpc::Status::OK;
    }

    ::grpc::Status GrpcManagerImpl::GetMonitorInfo(::grpc::ServerContext *context,
                                                   const ::google::protobuf::Empty *request,
                                                   ::monitor::proto::MonitorInfo *response)
    {
        *response = monitorInfos_;
        return ::grpc::Status::OK;
    }

}
