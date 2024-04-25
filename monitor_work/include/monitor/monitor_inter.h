#pragma once

#include <string>

// #define IF_GRPC

#ifdef IF_GRPC
#include "monitor_info.pb.h"
#include "monitor_info.grpc.pb.h"
#else
#include "rf_monitor_info.pb.h"
#endif

namespace monitor
{
    // 抽象类接口
    class MonitorInter
    {
    public:
        MonitorInter(){};
        virtual ~MonitorInter() {}
        virtual void UpdateOnce(monitor::proto::MonitorInfo *monitor_info) = 0; // 提取数据
        virtual void Stop() = 0;
    };
}
