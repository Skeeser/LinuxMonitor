#include "monitor/cpu_info_monitor.h"
#include "utils/read_file.h"
#include "utils/utils.h"

namespace monitor
{
    /*
    示例:
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 85
model name      : Intel(R) Xeon(R) Platinum 8269CY CPU @ 2.50GHz
stepping        : 7
microcode       : 0x1
cpu MHz         : 2499.998
cache size      : 36608 KB
physical id     : 0
siblings        : 2
core id         : 0
cpu cores       : 1
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 22
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss ht syscall nx pdpe1gb rdtscp lm constant_tsc rep_good nopl xtopology nonstop_tsc cpuid tsc_known_freq pni pclmulqdq ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lahf_lm abm 3dnowprefetch invpcid_single pti fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm mpx avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves arat avx512_vnni
bugs            : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf mds swapgs taa itlb_multihit
bogomips        : 4999.99
clflush size    : 64
cache_alignment : 64
address sizes   : 46 bits physical, 48 bits virtual
power management:
...
    */

    void CpuInfoMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        ReadFile cpuinfo_file(std::string("/proc/cpuinfo"));
        std::vector<std::string> one_cpuinfo;

        while (cpuinfo_file.ReadLineBySplitchar(&one_cpuinfo, ':') || !cpuinfo_file.IsEnd())
        {
            if (one_cpuinfo.empty())
            {
                continue;
            }

            if (one_cpuinfo[0] == "processor")
            {
                struct CpuInfo cpu_info;
                cpu_info.name = "cpu" + one_cpuinfo[1];
                // 往下读
                while (cpuinfo_file.ReadLineBySplitchar(&one_cpuinfo, ':'))
                {
                    // metric_info.http_server_requests = std::stof(metric_datas[1]);

                    if (one_cpuinfo[0] == "model name")
                    {
                        cpu_info.model_name = one_cpuinfo[1];
                    }
                    else if (one_cpuinfo[0] == "cpu MHz")
                    {
                        cpu_info.mhz = std::stof(one_cpuinfo[1]);
                    }
                    else if (one_cpuinfo[0] == "cache size")
                    {
                        cpu_info.cache_size_kb = std::stof(one_cpuinfo[1]);
                    }
                    else if (one_cpuinfo[0] == "cpu cores")
                    {
                        cpu_info.cores_num = std::stof(one_cpuinfo[1]);
                    }
                    else if (one_cpuinfo[0] == "power management")
                    {
                        one_cpuinfo.clear();
                        break;
                    }
                    one_cpuinfo.clear();
                }
                auto cpu_info_msg = monitor_info->add_cpu_info();
                cpu_info_msg->set_name(cpu_info.name);
                cpu_info_msg->set_model_name(cpu_info.model_name);
                cpu_info_msg->set_mhz(cpu_info.mhz);
                cpu_info_msg->set_cache_size_kb(cpu_info.cache_size_kb);
                cpu_info_msg->set_cores_num(cpu_info.cores_num);
            }
            one_cpuinfo.clear();
        }

        return;
    }
}