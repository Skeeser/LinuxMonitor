#pragma once

#include <mutex>
#include <string>
#include <thread>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h> // vastart va_end
#include <assert.h>
#include <sys/stat.h> //mkdir
#include "block_queue.h"
#include "log_buffer.h"

class Log
{
public:
    void init(int level = 1, const char *path = "../log",
              const char *suffix = ".log",
              int maxQueueCapacity = 1024);

    // 单例
    static Log *Instance();
    static void FlushLogThread(); // log的写入线程

    void write(int level, const char *format, ...);
    void flush(); // 缓冲区写入

    int GetLevel();
    void SetLevel(int level);
    bool IsOpen() { return isOpen_; }

private:
    Log();
    virtual ~Log();
    void AppendLogLevelTitle_(int level);
    void AsyncWrite_();

private:
    // 规定参数
    static const int LOG_PATH_LEN = 256;
    static const int LOG_NAME_LEN = 256;
    static const int MAX_LINES = 50000;

    // 路径和后缀名
    const char *path_;
    const char *suffix_;

    // buffer
    Buffer buff_;

    int MAX_LINES_;
    int lineCount_;
    int toDay_;

    bool isOpen_;
    int level_;
    bool isAsync_;

    FILE *fp_;
    std::unique_ptr<BlockDeque<std::string>> deque_;
    std::unique_ptr<std::thread> writeThread_;
    std::mutex mtx_;
};

// 添加宏定义
#define LOG_BASE(level, format, ...)                 \
    do                                               \
    {                                                \
        Log *log = Log::Instance();                  \
        if (log->IsOpen() && log->GetLevel <= level) \
        {                                            \
            log->write(level, format, ##__VA_ARGS__) \
        }                                            \
    } while (0);

#define LOG_DEBUG(format, ...)             \
    do                                     \
    {                                      \
        LOG_BASE(0, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_INFO(format, ...)              \
    do                                     \
    {                                      \
        LOG_BASE(1, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_WARN(format, ...)              \
    do                                     \
    {                                      \
        LOG_BASE(2, format, ##__VA_ARGS__) \
    } while (0);
#define LOG_ERROR(format, ...)             \
    do                                     \
    {                                      \
        LOG_BASE(3, format, ##__VA_ARGS__) \
    } while (0);
