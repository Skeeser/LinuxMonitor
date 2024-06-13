#include "monitor/os_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

namespace monitor
{
    void OsMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        ReadFile net_file(std::string("/etc/os-release"));
        std::vector<std::string> os_datas;
        while (net_file.ReadLineBySplitchar(&os_datas, '='))
        {
            if (os_datas[0].find("PRETTY_NAME") != std::string::npos)
            {
                struct OsInfo os_info;
                std::string name = os_datas[1];
                // 去除首尾双引号
                if (name.size() >= 2)
                {
                    name = name.substr(1, name.size() - 2);
                }
                os_info.name = name;

                auto os_msg = monitor_info->mutable_os_info();
                os_msg->set_name(os_info.name);
            }
            os_datas.clear();
        }
        return;
    }
} // namespace monitor
