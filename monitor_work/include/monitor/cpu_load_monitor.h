#pragma once

#include <string>

#include "monitor/monitor_inter.h"


namespace monitor
{
    class CpuLoadMonitor : public MonitorInter
    {
    public:
        CpuLoadMonitor() {}
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}

    private:
        float load_avg_1_;
        float load_avg_3_;
        float load_avg_15_;
    };

}
