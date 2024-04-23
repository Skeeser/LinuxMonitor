#include "rpc_framework_manager.h"

namespace monitor
{
    rpcFrameworkManagerImpl::rpcFrameworkManagerImpl()
    {
        // 初始化日志
        Log::Instance()->init(1, "./server_log", ".log", 1024);
        LOG_INFO("<-----------------SERVER---------------->");
    }

    rpcFrameworkManagerImpl::~rpcFrameworkManagerImpl() {}

    void rpcFrameworkManagerImpl::SetMonitorInfo(const ::monitor::proto::MonitorInfo *request,
                                                 ::google::protobuf::Empty *response,
                                                 ::google::protobuf::Closure *done)
    {
        monitorInfos_.Clear();
        // 赋值
        monitorInfos_ = *request;
        // 通过日志写入
        LOG_INFO("REQ:\n%s", request->DebugString().c_str());
        // 示例打印
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
        done->Run();
        // todo: 添加状态
        return;
    }

    void rpcFrameworkManagerImpl::GetMonitorInfo(const ::google::protobuf::Empty *request,
                                                 ::monitor::proto::MonitorInfo *response,
                                                 ::google::protobuf::Closure *done)
    {
        *response = monitorInfos_;
        done->Run();
        return;
    }

}
