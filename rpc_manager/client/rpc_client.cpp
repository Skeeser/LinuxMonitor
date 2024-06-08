#include "rpc_client.h"

namespace monitor
{
    RpcClient::RpcClient()
    {
        // 初始化日志
        Log::Instance()->init(1, "./client_log", ".log", 1024);
        LOG_INFO("<-----------------CLIENT---------------->");

        FILE *fp;
        fp = fopen("ip_address.txt", "r");
        char ipStr[50];
        bool isOpen = false;
        if (fp != nullptr && fgets(ipStr, 50, fp) != nullptr)
        {
            isOpen = true;
            fclose(fp);
            int pos = strlen(ipStr);
            ipStr[pos - 1] = '\0'; // 去除换行符
        }
        else
        {
            strcpy(ipStr, "localhost:50051");
        }

        auto channel = ::grpc::CreateChannel(ipStr, ::grpc::InsecureChannelCredentials());
        stubPtr_ = monitor::proto::GrpcManager::NewStub(channel);
    }

    RpcClient::~RpcClient()
    {
    }

    void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo &monito_info)
    {
        ::grpc::ClientContext context;
        ::google::protobuf::Empty response;
        ::grpc::Status status =
            stubPtr_->SetMonitorInfo(&context, monito_info, &response);
        if (status.ok())
        {
            LOG_INFO("Successfully set monitor info.")
        }
        else
        {
            // std::cout << status.error_details() << std::endl;
            // std::cout << "falied to connect !!!" << std::endl;
            LOG_ERROR("Detail: %s", status.error_details().c_str());
            LOG_ERROR("Falied to connect !!!");
        }
    }
    void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo *monito_info)
    {
        ::grpc::ClientContext context;
        ::google::protobuf::Empty request;
        ::grpc::Status status =
            stubPtr_->GetMonitorInfo(&context, request, monito_info);
        if (status.ok())
        {
            LOG_INFO("Successfully get monitor info.")
        }
        else
        {
            // std::cout << status.error_details() << std::endl;
            // std::cout << "falied to connect !!!" << std::endl;
            LOG_ERROR("Detail: %s", status.error_details().c_str());
            LOG_ERROR("Falied to connect !!!");
        }
    }
}