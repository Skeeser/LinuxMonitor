#include "grpc_manager.h"

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
        LOG_INFO("Soft_irq_size: %d", request->soft_irq_size());
        for (int i = 0; i < request->soft_irq_size(); i++)
        {
            LOG_INFO("cpu:%s hi:%d timer:%d net_tx:%d net_rx:%d block:%d irq_poll:%d",
                     request->soft_irq(i).cpu().c_str(),
                     request->soft_irq(i).hi(),
                     request->soft_irq(i).timer(),
                     request->soft_irq(i).net_tx(),
                     request->soft_irq(i).net_rx(),
                     request->soft_irq(i).block(),
                     request->soft_irq(i).irq_poll());
        }

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
