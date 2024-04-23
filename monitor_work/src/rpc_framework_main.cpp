#include <memory>
#include <thread>
#include <vector>

#include "client/rpc_framework_client.h"

#include "monitor/cpu_load_monitor.h"
#include "monitor/cpu_softirq_monitor.h"
#include "monitor/cpu_stat_monitor.h"
#include "monitor/mem_monitor.h"
#include "monitor/monitor_inter.h"
#include "monitor/net_monitor.h"

#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

void RpcRun()
{
    std::vector<std::shared_ptr<monitor::MonitorInter>> runners;

    runners.emplace_back(new monitor::CpuSoftIrqMonitor());
    runners.emplace_back(new monitor::CpuLoadMonitor());
    runners.emplace_back(new monitor::CpuStatMonitor());
    runners.emplace_back(new monitor::MemMonitor());
    runners.emplace_back(new monitor::NetMonitor());

    // 声明一个client
    network::EventLoop loop;
    monitor::RpcClient rpcClient(&loop);
    rpcClient.connect();

    // 获取环境变量
    char *name = getenv("USER");
    // 声明线程
    std::unique_ptr<std::thread> thread = nullptr;
    // 用lamda表达式定义线程
    thread = std::make_unique<std::thread>([&]()
                                           {
        while(true){
            monitor::proto::MonitorInfo monitorInfo;
            monitorInfo.set_name(std::string(name));
            for(auto & runner: runners){
                runner->UpdateOnce(&monitorInfo);
            }

            rpcClient.SetMonitorInfo(monitorInfo);
            // sleep 3秒
            std::this_thread::sleep_for(std::chrono::seconds(3));
        } });

    thread->join();
}

int main()
{

    RpcRun();
    return 0;
}