#pragma once

#include <string>
#include <unordered_map>

#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor
{
    class CpuInfoMonitor : public MonitorInter
    {
        // 软中断数据结构, 与protobuf对应
        struct CpuInfo
        {
            std::string name;    // cpu名称
            std::string model_name; // model名称
            float mhz;           // mhz大小
            float cache_size_kb; // cache大小
            float cores_num; // core数量
        };

    public:
        CpuInfoMonitor(){};
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}
    };
}