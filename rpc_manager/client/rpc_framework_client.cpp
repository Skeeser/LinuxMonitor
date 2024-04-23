#include "grpc_client.h"

namespace monitor
{
    RpcClient::RpcClient(network::EventLoop *loop, const network::InetAddress &serverAddr)
        : loop_(loop), client_(loop, serverAddr, "RpcClient"), channel_(new RpcChannel), stub(get_pointer(channel_))

    {
        // 初始化日志
        Log::Instance()->init(1, "./client_log", ".log", 1024);
        LOG_INFO("<-----------------CLIENT---------------->");

        client_.setConnectionCallback(std::bind(&RpcClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&RpcChannel, get_pointer(channel_), _1, _2));
    }

    RpcClient::RpcClient(EventLoop *loop)
        : loop_(loop), channel_(new RpcChannel), stub(get_pointer(channel_))

    {

        // 初始化日志
        Log::Instance()->init(1, "./client_log", ".log", 1024);
        LOG_INFO("<-----------------CLIENT---------------->");

        std::ifstream ifs("ip_address.txt");
        std::string line;
        std::getline(ifs, line);

        std::string ip;
        uint16_t port;
        if (ifs_.eof() || line.empty())
        {
            ip = "localhost";
            port = 50051;
        }
        else
        {
            // 解析ip和port
            int pos = line.find(':');
            if (pos != std::string::npos)
            {
                ip = line.sub_str(0, pos);
                port = std::stoi(line.sub_str(pos + 1, line.size() - pos - 1));
            }
            else
            {
                ip = "localhost";
                port = 50051;
            }
        }
        ifs.close();
        LOG_INFO("ip: %s, port: %d", ip.c_str(), port);
        network::InetAddress serverAddr(ip, port);

        client_(loop, serverAddr, "RpcClient");

        client_.setConnectionCallback(std::bind(&RpcClient::onConnection, this, _1));
        client_.setMessageCallback(std::bind(&RpcChannel, get_pointer(channel_), _1, _2));
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