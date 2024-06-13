#include "monitor/os_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

namespace monitor
{
    void OsMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        ReadFile net_file(std::string("/etc/os-release"));
        std::vector<std::string> os_datas;
        while (net_file.ReadEtcLine(&os_datas))
        {
            std::string key = os_datas[0];
            if (key == "PRETTY_NAME")
            {
                struct OsInfo os_info;

                os_info.name = os_datas[1];

                auto os_msg = monitor_info->mutable_os_info();
                os_msg->set_name(os_info.name);
            }
            os_datas.clear();
        }
        return;
    }
} // namespace monitor
