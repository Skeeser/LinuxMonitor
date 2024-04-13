#pragma once

#include <cstring> //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector>    //readv
#include <atomic>
#include <assert.h>

// 手搓了一个日志专门的缓冲buffer

class Buffer
{
public:
    Buffer(int initBufferSize = 1024);
    ~Buffer() = default;

    // 获取buffer信息
    size_t ReadableBytes() const;
    size_t WriteableBytes() const;
    size_t PrependableBytes() const;

    const char *Peek() const;         // 可读的头地址
    void EnsureWriteable(size_t len); // 确保可写,进行相关的空间操作
    void HasWritten(size_t len);      // 更新已写的下标

    void Retrieve(size_t len); // 取出数据
    void RetrieveUntil(const char *end);
    void RetrieveAll();
    std::string RetrieveAllToStr();

    const char *BeginWriteConst() const; // 可写的头地址
    char *BeginWrite();

    // 写入的函数
    void Append(const std::string &str);
    void Append(const char *str, size_t len);
    void Append(const void *data, size_t len);
    void Append(const Buffer &buff);

    // 将buff读写入文件中
    ssize_t ReadFd(int fd, int *Errno);
    ssize_t WriteFd(int fd, int *Errno);

private:
    char *
    BeginPtr_(); // 获取buffer的首地址
    const char *BeginPtr_() const;
    void MakeSpace_(size_t len); // 申请适配的空间

private:
    std::vector<char> buffer_;
    std::atomic<std::size_t> readPos_; // 原子变量
    std::atomic<std::size_t> writePos_;
};