#pragma once

#include <string>
#include <unordered_map>

#include "monitor/monitor_inter.h"
#include "monitor_info.grpc.pb.h"
#include "monitor_info.pb.h"

namespace monitor
{
    class MetricMonitor : public MonitorInter
    {
        struct MetricInfo
        {
            float http_server_requests;
            float executor_pool_max;
            float tomcat_sessions_created;
            float disk_total;
            float disk_free;
            float tomcat_sessions_expired;
            float tasks_scheduled_execution;
            float executor_queue_remaining;
            float executor_queued;
            float jvm_memory_used;
            float http_server_requests_active;
            float jvm_threads_daemon;
            float tomcat_sessions_active_current;
            float tomcat_sessions_active_max;
            float system_cpu_usage;
            float jvm_memory_usage_after_gc;
            float jvm_threads_peak;
            float process_cpu_usage;
            float jvm_threads_live;
            float executor_pool_size;
            float jvm_gc_overhead;
            float jvm_threads_started;
            float executor_completed;
            float executor_active;
            float tasks_scheduled_execution_active;
        };

    public:
        MetricMonitor() {}
        void UpdateOnce(monitor::proto::MonitorInfo *monitor_info);
        void Stop() override {}
    };

}