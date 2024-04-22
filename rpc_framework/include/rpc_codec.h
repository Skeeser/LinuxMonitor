#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

#include "rpc.pb.h"

namespace network
{
    class Buffer;
    class TcpConnection;
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    class RpcMessage;
    typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
    typedef std::shared_ptr<RpcMessage> RpcMessagePtr;
    extern const char rpctag[]; // = "RPC0";

    typedef std::function<void(const TcpConnectionPtr &, const RpcMessagePtr &)>
        ProtobufMessageCallback;

    // wire format
    //
    // Field     Length  Content
    //
    // size      4-byte  N+8
    // "RPC0"    4-byte
    // payload   N-byte
    // checksum  4-byte  adler32 of "RPC0"+payload

    // 编码类
    class RpcCodec
    {
    public:
        // 定义常数
        const static int kHeaderLen = sizeof(int32_t);
        const static int kChecksumLen = sizeof(int32_t);
        const static int kMaxMessageLen =
            64 * 1024 * 1024; // same as codec_stream.h kDefaultTotalBytesLimit
        enum ErrorCode
        {
            kNoError = 0,
            kInvalidLength,
            kCheckSumError,
            kInvalidNameLen,
            kUnknownMessageType,
            kParseError,
        };

        explicit RpcCodec(const ProtobufMessageCallback &messageCb) : messageCallback_(messageCb) {}
        ~RpcCodec() {}
        // 通过tcp发送msg
        void send(const TcpConnectionPtr &conn, const ::google::protobuf::Message &message);
        // 将可放入的buf内容放入msg中, 并调用msg回调函数
        void onMessage(const TcpConnectionPtr &conn, Buffer *buf);
        // 从buffer中赋值给msg, 不包含size段
        bool parseFromBuffer(const void *buf, int len,
                             ::google::protobuf::Message *message);
        ErrorCode parse(const char *buf, int len,
                        ::google::protobuf::Message *message);

        // 将msg数据序列化到buff中
        int serializeToBuffer(const google::protobuf::Message &message, Buffer *buf);

        // 将message内容复制到空白的buffer中, 包括size "rpc0" 序列化的内容 checksum
        void fillEmptyBuffer(Buffer *buf, const google::protobuf::Message &message);

    private:
        ProtobufMessageCallback messageCallback_;
        int kMinMessageLen = 4;
        const std::string tag_ = "RPC0";
    };
}