#include "monitor/metric_monitor.h"
#include "utils/read_file.h"

namespace monitor
{
    static constexpr float KBToGB = 1000 * 1000;

    void MetricMonitor::UpdateOnce(monitor::proto::MonitorInfo *monitor_info)
    {
        // 下面要改成metric所在的目录  建议直接挂载到build下面, 就不用绝对路径了
        ReadFile metric_file("/work/metric/metrics.txt");
        struct MetricInfo metric_info;
        std::vector<std::string> metric_datas;

        while (metric_file.ReadLine(&metric_datas))
        {
            // std::cout << "----> " << std::endl;
            if (metric_datas[0] == "http.server.requests")
            {
                metric_info.http_server_requests = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.pool.max")
            {
                metric_info.executor_pool_max = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tomcat.sessions.created")
            {
                metric_info.tomcat_sessions_created = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "disk.total")
            {
                metric_info.disk_total = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "disk.free")
            {
                metric_info.disk_free = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tomcat.sessions.expired")
            {
                metric_info.tomcat_sessions_expired = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tasks.scheduled.execution")
            {
                metric_info.tasks_scheduled_execution = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.queue.remaining")
            {
                metric_info.executor_queue_remaining = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.queued")
            {
                metric_info.executor_queued = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.memory.used")
            {
                metric_info.jvm_memory_used = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "http.server.requests.active")
            {
                metric_info.http_server_requests_active = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.threads.daemon")
            {
                metric_info.jvm_threads_daemon = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tomcat.sessions.active.current")
            {
                metric_info.tomcat_sessions_active_current = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tomcat.sessions.active.max")
            {
                metric_info.tomcat_sessions_active_max = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "system.cpu.usage")
            {
                metric_info.system_cpu_usage = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.memory.usage.after.gc")
            {
                metric_info.jvm_memory_usage_after_gc = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.threads.peak")
            {
                metric_info.jvm_threads_peak = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "process.cpu.usage")
            {
                metric_info.process_cpu_usage = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.threads.live")
            {
                metric_info.jvm_threads_live = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.pool.size")
            {
                metric_info.executor_pool_size = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.gc.overhead")
            {
                metric_info.jvm_gc_overhead = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "jvm.threads.started")
            {
                metric_info.jvm_threads_started = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.completed")
            {
                metric_info.executor_completed = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "executor.active")
            {
                metric_info.executor_active = std::stof(metric_datas[1]);
            }
            else if (metric_datas[0] == "tasks.scheduled.execution.active")
            {
                metric_info.tasks_scheduled_execution_active = std::stof(metric_datas[1]);
            }

            metric_datas.clear();
        }

        auto metric_detail = monitor_info->mutable_metric_info();

        metric_detail->set_http_server_requests(metric_info.http_server_requests);
        metric_detail->set_executor_pool_max(metric_info.executor_pool_max);
        metric_detail->set_tomcat_sessions_created(metric_info.tomcat_sessions_created);
        metric_detail->set_disk_total(metric_info.disk_total);
        metric_detail->set_disk_free(metric_info.disk_free);
        metric_detail->set_tomcat_sessions_expired(metric_info.tomcat_sessions_expired);
        metric_detail->set_tasks_scheduled_execution(metric_info.tasks_scheduled_execution);
        metric_detail->set_executor_queue_remaining(metric_info.executor_queue_remaining);
        metric_detail->set_executor_queued(metric_info.executor_queued);
        metric_detail->set_jvm_memory_used(metric_info.jvm_memory_used);
        metric_detail->set_http_server_requests_active(metric_info.http_server_requests_active);
        metric_detail->set_jvm_threads_daemon(metric_info.jvm_threads_daemon);
        metric_detail->set_tomcat_sessions_active_current(metric_info.tomcat_sessions_active_current);
        metric_detail->set_tomcat_sessions_active_max(metric_info.tomcat_sessions_active_max);
        metric_detail->set_system_cpu_usage(metric_info.system_cpu_usage);
        metric_detail->set_jvm_memory_usage_after_gc(metric_info.jvm_memory_usage_after_gc);
        metric_detail->set_jvm_threads_peak(metric_info.jvm_threads_peak);
        metric_detail->set_process_cpu_usage(metric_info.process_cpu_usage);
        metric_detail->set_jvm_threads_live(metric_info.jvm_threads_live);
        metric_detail->set_executor_pool_size(metric_info.executor_pool_size);
        metric_detail->set_jvm_gc_overhead(metric_info.jvm_gc_overhead);
        metric_detail->set_jvm_threads_started(metric_info.jvm_threads_started);
        metric_detail->set_executor_completed(metric_info.executor_completed);
        metric_detail->set_executor_active(metric_info.executor_active);
        metric_detail->set_tasks_scheduled_execution_active(metric_info.tasks_scheduled_execution_active);

        return;
    }
} // namespace monitor
