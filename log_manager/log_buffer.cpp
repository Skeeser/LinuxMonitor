#include "log_buffer.h"

Buffer::Buffer(int initBufferSize) : buffer_(initBufferSize), readPos_(0), writePos_(0) {}

size_t Buffer::ReadableBytes() const
{
    return writePos_ - readPos_;
}

size_t Buffer::WriteableBytes() const
{
    return buffer_.size() - writePos_;
}

size_t Buffer::PrependableBytes() const
{
    return readPos_;
}

char *Buffer::BeginPtr_()
{
    return &*buffer_.begin();
}

const char *Buffer::BeginPtr_() const
{
    return &*buffer_.begin();
}

void Buffer::MakeSpace_(size_t len)
{
    if (WriteableBytes() + PrependableBytes() < len) // 太大了
    {
        buffer_.resize(writePos_ + len + 1);
    }
    else
    {
        size_t readable = ReadableBytes();
        auto beginptr = BeginPtr_();
        // 将中间那段移到开头
        std::copy(beginptr + readPos_, beginptr + writePos_, beginptr);
        readPos_ = 0;
        writePos_ = readPos_ + readable;
        assert(readable == ReadableBytes());
    }
}

const char *Buffer::Peek() const
{
    return BeginPtr_() + readPos_;
}

void Buffer::EnsureWriteable(size_t len)
{
    if (WriteableBytes() < len)
    {
        MakeSpace_(len);
    }
    assert(WriteableBytes() >= len);
}

void Buffer::HasWritten(size_t len)
{
    writePos_ += len;
}

void Buffer::Retrieve(size_t len)
{
    assert(len <= ReadableBytes());
    readPos_ += len;
}
void Buffer::RetrieveUntil(const char *end)
{
    assert(Peek() <= end);
    Retrieve(end - Peek());
}
void Buffer::RetrieveAll()
{
    // 将buffer清零
    bzero(&buffer_[0], buffer_.size());
    readPos_ = 0;
    writePos_ = 0;
}
std::string Buffer::RetrieveAllToStr()
{
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}

const char *Buffer::BeginWriteConst() const
{
    return BeginPtr_() + writePos_;
}

char *Buffer::BeginWrite()
{
    return BeginPtr_() + writePos_;
}

void Buffer::Append(const char *str, size_t len)
{
    assert(str);
    EnsureWriteable(len); // 申请足够空间
    std::copy(str, str + len, BeginWrite());
    HasWritten(len); // 更新已写
}

void Buffer::Append(const std::string &str)
{
    Append(str.data(), str.length());
}

void Buffer::Append(const void *data, size_t len)
{
    assert(data);
    Append(static_cast<const char *>(data), len);
}
void Buffer::Append(const Buffer &buff)
{
    Append(buff.Peek(), buff.ReadableBytes());
}

ssize_t Buffer::ReadFd(int fd, int *Errno)
{
    char buff[65535];
    struct iovec iov[2];
    const size_t writeable = WriteableBytes();

    // 分散读, 确定能读完
    iov[0].iov_base = BeginWrite();
    iov[0].iov_len = writeable;
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if (len < 0)
    {
        *Errno = errno;
    }
    else if (static_cast<size_t>(len) <= writeable)
    {
        writePos_ += len;
    }
    else
    {
        writePos_ = buffer_.size(); // 默认已经全部写完
        Append(buff, len - writeable);
    }

    return len;
}

ssize_t Buffer::WriteFd(int fd, int *Errno)
{
    size_t readable = ReadableBytes();
    ssize_t len = write(fd, Peek(), readable);
    if (len < 0)
    {
        *Errno = errno;
        return len;
    }
    readPos_ += len;
    return len;
}