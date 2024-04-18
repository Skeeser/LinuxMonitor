#include "rpc_codec.h"

#include <google/protobuf/message.h>
#include "network/Endian.h"
#include "network/TcpConnection.h"
#include "utils.h"

namespace network
{

    void RpcCodec::send(const TcpConnectionPtr &conn, const ::google::protobuf::Message &message)
    {
        Buffer buff;
        fillEmptyBuffer(&buff, message);
        conn->send(&buff);
    }

    void RpcCodec::fillEmptyBuffer(Buffer *buf, const google::protobuf::Message &message)
    {
        // 确保buffer为空
        assert(buf->readableBytes() == 0);
        // 先添加tag
        buf->append(tag_);
        // 序列化
        int byte_size = serializeToBuffer(message, buf);
        // 获取已经写入的大小
        int32_t checksum = Utils::checkSum(buf->peek(), static_cast<int>(buf->readableBytes()));
        buf->appendInt32(checksum);
        assert(buf->readableBytes() == tag_.size() + byte_size + kChecksumLen);
        (void)byte_size;
        // 主机顺序变为网络顺序
        int32_t len = sockets::hostToNetwork32(static_cast<int32_t>(buf->readableBytes()));
        buf->prepend(&len, sizeof len);
    }

    void RpcCodec::onMessage(const TcpConnectionPtr &conn, Buffer *buf)
    {
        while (buf->readableBytes() >= static_cast<uint32_t>(kMinMessageLen + kHeaderLen))
        {
            const int32_t len = buf->peekInt32();
            if (len > kMaxMessageLen || len < kMinMessageLen)
            {
                break;
            }
            else if (buf->readableBytes() >= size_t(kHeaderLen + len))
            {
                RpcMessagePtr message(new RpcMessage());
                ErrorCode errorCode = parse(buf->peek() + kHeaderLen, len, message.get());

                if (errorCode == kNoError)
                {
                    messageCallback_(conn, message);
                    buf->retrieve(kHeaderLen + len);
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    bool RpcCodec::parseFromBuffer(const void *buf, int len,
                                   google::protobuf::Message *message)
    {
        return message->ParseFromArray(buf, len);
    }
    ErrorCode RpcCodec::parse(const char *buf, int len,
                              ::google::protobuf::Message *message)
    {
        ErrorCode error = kNoError;

        // 检验校验和
        if (Utils::validateChecksum(buf, len, kChecksumLen))
        {
            // 验证tag是否正确
            if (memcmp(buf, tag_.data(), tag_.size()) == 0)
            {
                const char *data = buf + tag_.size();
                int32_t dataLen = len - kChecksumLen - static_cast<int>(tag_.size());
                if (parseFromBuffer(data, dataLen, message))
                {
                    error = kNoError;
                }
                else
                {
                    error = kParseError;
                }
            }
            else
            {
                error = kUnknownMessageType;
            }
        }
        else
        {
            errpr = kCheckSumError;
        }

        return error;
    }

    int RpcCodec::serializeToBuffer(const google::protobuf::Message &message, Buffer *buf);

}