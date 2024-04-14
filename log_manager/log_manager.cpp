#include "log_manager.h"

void Log::init(int level, const char *path,
               const char *suffix,
               int maxQueueCapacity)
{
    isOpen_ = true;
    level_ = level;

    // 判断是否开启异步
    if (maxQueueCapacity > 0) // 开启
    {
        isAsync_ = true;

        // init异步,  申请阻塞队列
        if (!deque_)
        {
            std::unique_ptr<BlockDeque<std::string>> newDeque(new BlockDeque<std::string>);
            deque_ = std::move(newDeque);
            std::unique_ptr<std::thread> newThread(new std::thread(FlushLogThread));
            writeThread_ = std::move(newThread);
        }
    }
    else
    {
        isAsync_ = false;
    }

    // 初始化成员变量
    lineCount_ = 0;
    // 时间
    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    // 路径
    path_ = path;
    suffix_ = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s",
             path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
    toDay_ = t.tm_mday;

    // 加锁进行文件处理
    {
        std::lock_guard<std::mutex> locker(mtx_);
        buff_.RetrieveAll();
        if (fp_)
        {
            flush();
            fclose(fp_);
        }

        fp_ = fopen(fileName, "a");
        if (fp_ == nullptr)
        {
            mkdir(path_, 0777);
            fp_ = fopen(fileName, "a");
        }
        // std::cout << path_ << std::endl;
        assert(fp_ != nullptr);
    }
}

// 单例
Log *Log::Instance()
{
    static Log instance;
    return &instance;
}

void Log::FlushLogThread()
{
    Log::Instance()->AsyncWrite_();
}

void Log::write(int level, const char *format, ...)
{
    // 获取时间
    struct timeval now = {0, 0};
    gettimeofday(&now, nullptr); // 与time()区别是能得到微秒级结果
    time_t tSec = now.tv_sec;
    struct tm *sysTime = localtime(&tSec);
    struct tm t = *sysTime;
    va_list vaList; // 参数列表

    // 日志日期, 日志行数
    if (toDay_ != t.tm_mday || (lineCount_ && (lineCount_ % MAX_LINES == 0)))
    {
        std::unique_lock<std::mutex> locker(mtx_);
        locker.unlock();
        // 重新生成日志文件
        char newFile[LOG_NAME_LEN];
        char tail[36] = {0};
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
        if (toDay_ != t.tm_mday)
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);
            toDay_ = t.tm_mday;
            lineCount_ = 0;
        }
        else
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount_ / MAX_LINES), suffix_);
        }

        // 文件操作加锁
        locker.lock();
        flush();
        fclose(fp_);
        fp_ = fopen(newFile, "a");
        assert(fp_ != nullptr);
    }

    // 写入操作
    {
        std::unique_lock<std::mutex> locker(mtx_);
        lineCount_++;
        // 确保能写入
        buff_.EnsureWriteable(128);
        // 生成日志行的头
        int n = snprintf(buff_.BeginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                         t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                         t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);
        buff_.HasWritten(n);
        AppendLogLevelTitle_(level);

        va_start(vaList, format);
        // todo: 怎么确保能写入
        int m = vsnprintf(buff_.BeginWrite(), buff_.WriteableBytes(), format, vaList);
        va_end(vaList);
        buff_.HasWritten(m);
        buff_.Append("\n\0", 2);

        if (isAsync_ && deque_ && !deque_->full())
        {
            deque_->push_back(buff_.RetrieveAllToStr());
        }
        else
        {
            fputs(buff_.Peek(), fp_);
        }
        // 清空
        buff_.RetrieveAll();
    }
}

void Log::flush()
{
    if (isAsync_)
    {
        deque_->flush();
    }
    fflush(fp_);
}

int Log::GetLevel()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return level_;
}
void Log::SetLevel(int level)
{
    std::lock_guard<std::mutex> locker(mtx_);
    level_ = level;
}

Log::Log()
{
    lineCount_ = 0;
    isAsync_ = false;
    writeThread_ = nullptr;
    deque_ = nullptr;
    toDay_ = 0;
    fp_ = nullptr;
}

Log::~Log()
{
    // 线程关闭
    if (writeThread_ && writeThread_->joinable())
    {
        while (!deque_->empty())
        {
            deque_->flush();
        }
        deque_->close();
        writeThread_->join();
    }

    if (fp_)
    {
        std::lock_guard<std::mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }
}

void Log::AppendLogLevelTitle_(int level)
{
    switch (level)
    {
    case 0:
        buff_.Append("[debug]: ", 9);
        break;
    case 1:
        buff_.Append("[info] : ", 9);
        break;
    case 2:
        buff_.Append("[warn] : ", 9);
        break;
    case 3:
        buff_.Append("[error]: ", 9);
        break;
    default:
        buff_.Append("[info] : ", 9);
        break;
    }
}

void Log::AsyncWrite_()
{
    // 异步写入线程任务
    std::string str = "";
    while (deque_->pop(str)) // pop()是阻塞的, 直到队列关闭才会返回false
    {
        std::lock_guard<std::mutex> locker(mtx_);
        // std::cout << (bool)(fp_ == nullptr) << std::endl;
        fputs(str.c_str(), fp_);
        // fflush(fp_);
    }
}