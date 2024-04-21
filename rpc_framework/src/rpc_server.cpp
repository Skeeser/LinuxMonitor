#include "rpc_server.h"

#include <cassert>

#include "log_manager.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

RpcServer::RpcServer(EventLoop *loop, const InetAddress &listenAddr);

void RpcServer::setThreadNum(int numThreads) { server_.setThreadNum(numThreads); }

void RpcServer::registerService(::google::protobuf::Service *);
void RpcServer::start();

void RpcServer::onConnection_(const TcpConnectionPtr &conn);
