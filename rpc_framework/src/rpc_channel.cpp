#include "rpc_channel.h"

#include <cassert>

#include <google/protobuf/descriptor.h>

#include "rpc.pb.h"
// #include "log_manager.h"

namespace network
{
    RpcChannel::RpcChannel() : codec_(std::bind(&RpcChannel::onRpcMessage_, this, std::placeholders::_1,
                                                std::placeholders::_2)),
                               services_(nullptr) {}

    RpcChannel::RpcChannel(const TcpConnectionPtr &conn) : codec_(std::bind(&RpcChannel::onRpcMessage_, this, std::placeholders::_1,
                                                                            std::placeholders::_2)),
                                                           conn_(conn),
                                                           services_(nullptr)
    {
    }

    RpcChannel::~RpcChannel()
    {
    }

    void RpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor *method,
                                ::google::protobuf::RpcController *controller,
                                const ::google::protobuf::Message *request,
                                ::google::protobuf::Message *response,
                                ::google::protobuf::Closure *done)
    {
        // 定义的protobuf msg
        RpcMessage message;
        message.set_type(REQUEST);
        int64_t id = id_.fetch_add(1) + 1;
        message.set_id(id);
        message.set_service(method->service()->full_name());
        message.set_method(method->name());
        message.set_request(request->SerializeAsString());

        OutstandingCall out = {response, done};
        {
            std::unique_lock<std::mutex> lock(mutex_);
            outstandings_[id] = out;
        }
        // 通过tcp连接发送msg
        codec_.send(conn_, message);
    }

    void RpcChannel::onMessage(const TcpConnectionPtr &conn, Buffer *buf)
    {
        codec_.onMessage(conn, buf);
    }

    void RpcChannel::onRpcMessage_(const TcpConnectionPtr &conn,
                                   const RpcMessagePtr &messagePtr)
    {
        // 检测连接是否改变
        assert(conn == conn_);
        RpcMessage &message = *messagePtr;
        // 判断消息类型
        if (message.type() == RESPONSE)
        {
            handleResponseMsg_(messagePtr);
        }
        else if (message.type() == REQUEST)
        {
            handleRequestMsg_(messagePtr);
        }
    }

    void RpcChannel::doneCallback_(::google::protobuf::Message *response, int64_t id)
    {
        std::unique_ptr<google::protobuf::Message> dd(response);
        RpcMessage message;
        message.set_type(RESPONSE);
        message.set_id(id);
        message.set_response(response->SerializeAsString());
        codec_.send(conn_, message);
    }

    void RpcChannel::handleResponseMsg_(const RpcMessagePtr &messagePtr)
    {
        // 取得消息
        RpcMessage &message = *messagePtr;
        int64_t id = message.id();
        OutstandingCall out = {NULL, NULL};

        {
            std::lock_guard<std::mutex> locker(mutex_);
            auto it = outstanding_.find(id);
            if (it != outstandings_.end())
            {
                // 从待完成response中删去
                out = it->second;
                outstandings_.erase(it);
            }
        }

        // 处理response
        if (out.response)
        {
            // 转移到智能指针中自动释放
            std::unique_ptr<::google::protobuf::Message> responsePtr(out.response);

            if (!out.response().empty())
            {
                out.response->ParseFromString(message.response());
            }

            if (out.done)
            {
                out.done->Run();
            }
        }
    }
    void RpcChannel::handleRequestMsg_(const TcpConnectionPtr &conn,
                                       const RpcMessagePtr &messagePtr)
    {
        RpcMessage &message = *messagePtr;
        ErrorCode error = WRONG_PROTO;
        // 判断有没有设置服务
        if (services_)
        {
            // 查找服务中有没有message中对应的服务
            auto it = services_.find(message.service());

            if (it != services_.end())
            {
                // 获取对应服务
                ::google::protobuf::Service *service = it->second;
                assert(service != nullptr);

                const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
                const google::protobuf::MethodDescriptor *method = desc->FindMethodByName(message.method());

                if (method)
                {
                    // 解析message
                    std::unique_ptr<::google::protobuf::Message> requestMsg(
                        service->GetRequestPrototype(method).New());
                    // 从message取出request
                    if (requestMsg->ParseFromString(message.request()))
                    {
                        // 声明response
                        ::google::protobuf::Message *responseMsg =
                            service->GetResponsePrototype(method).New();
                        int64_t id = message.id();
                        service->CallMethod(method, NULL, request.get(), responseMsg,
                                            NewCallback(this, &RpcChannel::doneCallback, response, id));
                        error = NO_ERROR;
                    }
                    else
                    {
                        error = INVALID_REQUEST;
                    }
                }
                else
                {
                    error = NO_METHOD;
                }
            }
            else
            {
                error = NO_SERVICE;
            }
        }
        else
        {
            error = NO_SERVICE;
        }
    }
}