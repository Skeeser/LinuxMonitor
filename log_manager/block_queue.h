#pragma once

#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>

// 阻塞队列
// 就是队列的读写操作都加互斥锁
// 对应的是无锁队列, 怎么实现?

template <class T>
class BlockDeque
{
public:
    explicit BlockDeque(size_t MaxCapacity = 1000);
    ~BlockDeque();

    // 队列的基本操作
    void clear();
    bool empty();
    bool full();
    void close();
    size_t size();
    size_t capacity();

    T front();
    T back();

    void push_back(const T &item);
    void push_front(const T &item);

    bool pop(T &item);
    bool pop(T &item, int timeout);

    void flush();

private:
    // 内部维护的双端队列
    std::deque<T> deq_;
    size_t capacity_;

    // 锁
    std::mutex mtx_;
    // 记录关闭状态
    bool isClose_;

    // 生产者和消费者条件变量
    std::condition_variable condConsumer_;
    std::condition_variable condProducer_;
};

template <class T>
BlockDeque<T>::BlockDeque(size_t MaxCapacity) : capacity_(MaxCapacity)
{
    assert(MaxCapacity > 0);
    isClose_ = false;
}
template <class T>
BlockDeque<T>::~BlockDeque()
{
    close();
}

// 队列的基本操作
template <class T>
void BlockDeque<T>::clear()
{
    std::lock_guard<std::mutex> locker(mtx_);
    deq_.clear();
}

template <class T>
bool BlockDeque<T>::empty()
{
    std::lock_guard<std::mutex> locker(mtx_); // 避免脏读
    return deq_.empty();
}

template <class T>
bool BlockDeque<T>::full()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size() >= capacity_;
}
template <class T>
void BlockDeque<T>::close()
{
    {
        std::lock_guard<std::mutex> locker(mtx_);
        deq_.clear();
        isClose_ = true;
    }

    // 唤醒所有线程结束
    condProducer_.notify_all();
    condConsumer_.notify_all();
}

template <class T>
size_t BlockDeque<T>::size()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.size();
}

template <class T>
size_t BlockDeque<T>::capacity()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return capacity_;
}

template <class T>
T BlockDeque<T>::front()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.front();
}

template <class T>
T BlockDeque<T>::back()
{
    std::lock_guard<std::mutex> locker(mtx_);
    return deq_.back();
}

template <class T>
void BlockDeque<T>::push_back(const T &item)
{
    // 生产者消费者模型
    std::unique_lock<std::mutex> locker(mtx_);
    while (deq_.size() >= capacity_)
    {
        // 生产者等待
        condProducer_.wait(locker);
    }
    deq_.push_back(item);
    // 唤醒消费者
    condConsumer_.notify_one();
}

template <class T>
void BlockDeque<T>::push_front(const T &item)
{
    // 生产者消费者模型
    std::unique_lock<std::mutex> locker(mtx_);
    while (deq_.size() >= capacity_)
    {
        // 生产者等待
        condProducer_.wait(locker);
    }
    deq_.push_front(item);
    // 唤醒消费者
    condConsumer_.notify_one();
}

template <class T>
bool BlockDeque<T>::pop(T &item)
{
    // 生产者消费者模型
    std::unique_lock<std::mutex> locker(mtx_);
    while (deq_.empty())
    {
        // 消费者等待
        condConsumer_.wait(locker);
        if (isClose_)
        {
            return false;
        }
    }
    item = deq_.front();
    deq_.pop_front();
    // 唤醒生产者
    condProducer_.notify_one();
    return true;
}

template <class T>
bool BlockDeque<T>::pop(T &item, int timeout)
{
    // 生产者消费者模型
    std::unique_lock<std::mutex> locker(mtx_);
    while (deq_.empty())
    {
        // 消费者等待一定时间
        if (condConsumer_.wait_for(locker, std::chrono::seconds(timeout)) == std::cv_status::timeout)
        {
            return false;
        }
        if (isClose_)
        {
            return false;
        }
    }
    item = deq_.front();
    deq_.pop_front();
    // 唤醒生产者
    condProducer_.notify_one();
    return true;
}

// 刷新, 让pop线程知道deque close, 以便关闭线程
template <class T>
void BlockDeque<T>::flush()
{
    condConsumer_.notify_one();
}