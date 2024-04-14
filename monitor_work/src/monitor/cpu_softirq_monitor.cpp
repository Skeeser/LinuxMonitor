#include "monitor/cpu_softirq_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

namespace monitor
{
    /*
    示例:
                        CPU0       CPU1
              HI:          0          0
           TIMER:    9993077   10540664
          NET_TX:         16         11
          NET_RX:    1041671    1157237
           BLOCK:          0     524737
        IRQ_POLL:          0          0
         TASKLET:         41       3246
           SCHED:    9571716   10130516
         HRTIMER:        333          0
             RCU:    7337518    7312720

    */

    void CpuSoftIrqMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        ReadFile softirqs_file(std::string("/proc/softirqs"));
        std::vector<std::string> one_softirq;
        std::vector<std::vector<std::string>> softirq;
        while (softirqs_file.ReadLine(&one_softirq))
        {
            softirq.push_back(one_softirq);
            one_softirq.clear();
        }

        for (int i = 0; i < softirq[0].size() - 1; i++)
        {
            // 将文件的信息赋值到结构体中
            std::string name = softirq[0][i];
            struct SoftIrq info;
            info.cpu_name = name;
            info.hi = std::stoll(softirq[1][i + 1]);
            info.timer = std::stoll(softirq[2][i + 1]);
            info.net_tx = std::stoll(softirq[3][i + 1]);
            info.net_rx = std::stoll(softirq[4][i + 1]);
            info.block = std::stoll(softirq[5][i + 1]);
            info.irq_poll = std::stoll(softirq[6][i + 1]);
            info.tasklet = std::stoll(softirq[7][i + 1]);
            info.sched = std::stoll(softirq[8][i + 1]);
            info.hrtimer = std::stoll(softirq[9][i + 1]);
            info.rcu = std::stoll(softirq[10][i + 1]);
            info.timepoint = boost::chrono::steady_clock::now();

            // 寻找之前存的cpu的信息
            auto iter = cpu_softirqs_.find(name);
            if (iter != cpu_softirqs_.end())
            {
                auto &old = iter->second;
                double period = Utils::SteadyTimeSecond(info.timepoint, old.timepoint);
                // 计算赋值进protobuf
                auto one_softirq_msg = monitor_info->add_soft_irq();
                one_softirq_msg->set_cpu(info.cpu_name);
                one_softirq_msg->set_hi((info.hi - old.hi) / period);
                one_softirq_msg->set_timer((info.timer - old.timer) / period);
                one_softirq_msg->set_net_tx((info.net_tx - old.net_tx) / period);
                one_softirq_msg->set_net_rx((info.net_rx - old.net_rx) / period);
                one_softirq_msg->set_block((info.block - old.block) / period);
                one_softirq_msg->set_irq_poll((info.irq_poll - old.irq_poll) / period);
                one_softirq_msg->set_tasklet((info.tasklet - old.tasklet) / period);
                one_softirq_msg->set_sched((info.sched - old.sched) / period);
                one_softirq_msg->set_hrtimer((info.hrtimer - old.hrtimer) / period);
                one_softirq_msg->set_rcu((info.rcu - old.rcu) / period);
            }

            // 将对应信息存进哈希表
            cpu_softirqs_[name] = info;
        }
        return;
    }
}