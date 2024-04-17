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
        }
    bool RpcCodec::parseFromBuffer(const void *buf, int len,
                                   google::protobuf::Message *message);
    int RpcCodec::serializeToBuffer(const google::protobuf::Message &message, Buffer *buf);
    ErrorCode RpcCodec::parse(const char *buf, int len,
                              ::google::protobuf::Message *message);

    static int32_t RpcCodec::checksum(const void *buf, int len);
    static bool RpcCodec::validateChecksum(const char *buf, int len);
    static int32_t RpcCodec::asInt32(const char *buf);
}