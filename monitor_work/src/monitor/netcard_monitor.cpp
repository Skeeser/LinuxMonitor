#include "monitor/netcard_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"
#include <dirent.h>

namespace monitor
{
    void NetcardMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("/sys/class/net/")) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                // 读取目录
                if (ent->d_name[0] != '.')
                {
                    struct NetcardInfo netcard_info;
                    netcard_info.name = ent->d_name;

                    std::ifstream ifs("/sys/class/net/" + netcard_info.name + "/address");
                    std::string macAddress;
                    if (ifs.is_open())
                    {
                        std::getline(ifs, macAddress);
                        netcard_info.mac_address = macAddress;
                        ifs.close();
                    }

                    ifs.open("/sys/class/net/" + netcard_info.name + "/mtu");
                    std::string mtu;
                    if (ifs.is_open())
                    {
                        std::getline(ifs, mtu);
                        netcard_info.mtu = std::stof(mtu);
                        ifs.close();
                    }

                    auto one_netcard_msg = monitor_info->add_netcard_info();
                    one_netcard_msg->set_name(netcard_info.name);
                    one_netcard_msg->set_mac_address(netcard_info.mac_address);
                    one_netcard_msg->set_mtu(netcard_info.mtu);
                }
            }
            closedir(dir);
        }

        return;
    }
} // namespace monitor
