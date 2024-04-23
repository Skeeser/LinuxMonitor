#pragma once

#include <string>
#include <unordered_map>

#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"



namespace monitor
{
    class CpuSoftIrqMonitor : public MonitorInter
    {
        // 软中断数据结构, 与protobuf对应
        struct SoftIrq
        {
            std::string cpu_name;
            int64_t hi;
            int64_t timer;
            int64_t net_tx;
            int64_t net_rx;
            int64_t block;
            int64_t irq_poll;
            int64_t tasklet;
            int64_t sched;
            int64_t hrtimer;
            int64_t rcu;
            boost::chrono::steady_clock::time_point timepoint;
        };

    public:
        CpuSoftIrqMonitor(){};
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}

    private:
        // 各个cpu对应的SoftIrq
        std::unordered_map<std::string, struct SoftIrq> cpu_softirqs_;
    };
}