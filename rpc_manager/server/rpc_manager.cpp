#include "rpc_manager.h"

namespace monitor{
GrpcManagerImpl::GrpcManagerImpl() {}

GrpcManagerImpl::~GrpcManagerImpl() {}

::grpc::Status SetMonitorInfo(::grpc::ServerContext* context,
        const ::monitor::proto::MonitorInfo* request,
        ::google::protobuf::Empty* response){
    monitor_infos_.Clear();
    // 赋值
    monitor_infos_ = *request;
    // todo: 通过日志写入
    std::cout << "into" << request->soft_irq_size() << std::endl;
    
    return ::grpc::Status::OK;

}


::grpc::Status GetMonitorInfo(::grpc::ServerContext* context,
                                const ::google::protobuf::Empty* request,
                                ::monitor::proto::MonitorInfo* response);



}


