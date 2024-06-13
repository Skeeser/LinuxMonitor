#pragma once

#include <string>
#include <unordered_map>
#include <boost/chrono.hpp>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor
{
    class OsMonitor : public MonitorInter
    {
        struct OsInfo
        {
            std::string name;
        };

    public:
        OsMonitor() {}
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}

    };

} // namespace monitor