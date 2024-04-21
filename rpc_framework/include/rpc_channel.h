#pragma once

#include <map>
#include <mutex>

#include <google/protobuf/service.h>

#include "rpc_codec.h"
#include "rpc.pb.h"

namespace google
{
    namespace protobuf
    {

        // Defined in other files.
        class Descriptor;        // descriptor.h
        class ServiceDescriptor; // descriptor.h
        class MethodDescriptor;  // descriptor.h
        class Message;           // message.h

        class Closure;

        class RpcController;
        class Service;

    } // namespace protobuf
} // namespace google

namespace network
{
    class RpcChannel : public ::google::protobuf::RpcChannel
    {
    public:
        RpcChannel();

        explicit RpcChannel(const TcpConnectionPtr &conn);

        ~RpcChannel() override;

        void setConnection(const TcpConnectionPtr &conn) { conn_ = conn; }

        // 设置服务
        void setServices(const std::map<std::string, ::google::protobuf::Service *> *services)
        {
            services_ = services;
        }

        void CallMethod(const ::google::protobuf::MethodDescriptor *method,
                        ::google::protobuf::RpcController *controller,
                        const ::google::protobuf::Message *request,
                        ::google::protobuf::Message *response,
                        ::google::protobuf::Closure *done) override;

        void onMessage(const TcpConnectionPtr &conn, Buffer *buf);

    private:
        void onRpcMessage_(const TcpConnectionPtr &conn,
                           const RpcMessagePtr &messagePtr);
        // 结束回调, 发送response
        void doneCallback_(::google::protobuf::Message *response, int64_t id);

        // 处理请求和回应的消息
        void handleResponseMsg_(const RpcMessagePtr &messagePtr);
        void handleRequestMsg_(const TcpConnectionPtr &conn,
                               const RpcMessagePtr &messagePtr);

        struct OutstandingCall
        {
            ::google::protobuf::Message *response;
            ::google::protobuf::Closure *done;
        };

        RpcCodec codec_;
        TcpConnectionPtr conn_;
        std::atomic<int64_t> id_;

        std::mutex mutex_;
        // 注意线程安全, 存储id和其对应的待回应的消息类型
        std::map<int64_t, OutstandingCall> outstandings_;

        const std::map<std::string, ::google::protobuf::Service *> *services_;
    };
    typedef std::shared_ptr<RpcChannel> RpcChannelPtr;
}