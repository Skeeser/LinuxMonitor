#pragma once

#include <string>

#include "monitor_info.grpc.pb.h"
namespace monitor
{
    // 抽象类接口
    class MonitorInter
    {
    public:
        MonitorInter(){};
        virtual ~MonitorInter() {}
    }
}
