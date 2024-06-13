#pragma once

#include <string>
#include <unordered_map>
#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor
{
    class NetcardMonitor : public MonitorInter
    {
        struct NetcardInfo
        {
            std::string name;        // 网卡名称
            std::string mac_address; // mac地址
            float mtu;               // mtu大小
        };

    public:
        NetcardMonitor() {}
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}
    };
} // namespace monitor