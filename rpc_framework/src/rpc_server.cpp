#include "rpc_server.h"

#include <cassert>

#include "log_manager.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

#include "rpc_channel.h"

namespace network
{
    RpcServer::RpcServer(EventLoop *loop, const InetAddress &listenAddr)
        : server_(loop, listenAddr, "RpcServer")
    {
        // 初始化日志系统
        Log::Instance()->init(1, "./server_log", ".log", 1024);

        // 调用tcp server函数, 将本类的onConnection设置为connect的回调函数
        server_.setConnectionCallback(std::bind(&RpcServer::onConnection_, this, _1));
    }

    void RpcServer::registerService(::google::protobuf::Service *service)
    {
        // 添加服务
        const google::protobuf::ServiceDescriptor *desc = service->GetDescriptor();
        services_[desc->full_name()] = service;
    }
    void RpcServer::start()
    {
        server_.start();
    }

    void RpcServer::onConnection_(const TcpConnectionPtr &conn)
    {
        LOG_INFO("RpcServer - %s -> %s is %s",
                 conn->peerAddress().toIpPort().c_str(),
                 conn->localAddress().toIpPort().c_str(),
                 (conn->connected() ? "UP" : "DOWN"));
        if (conn->connected())
        {
            // 声明一个channel
            RpcChannelPtr channel(new RpcChannel(conn));
            // 设置全部服务
            channel->setServices(&services_);
            // 设置消息的回调函数
            conn->setMessageCallback(std::bind(&RpcChannel::onMessage, get_pointer(channel), _1, _2));
            conn->setContext(channel);
        }
        else
        {
            conn->setContext(RpcChannelPtr());
        }
    }
}