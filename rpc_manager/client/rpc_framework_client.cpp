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

        client_.setConnectionCallback(std::bind(&RpcClient::onConnection_, this, _1));
        client_.setMessageCallback(std::bind(&RpcChannel, get_pointer(channel_), _1, _2));
    }

    RpcClient::~RpcClient()
    {
    }

    /*
    ::google::protobuf::RpcController *controller,
    const ::monitor::proto::MonitorInfo *request,
    ::google::protobuf::Empty *response,
    ::google::protobuf::Closure *done
    */
    void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo &monito_info)
    {

        ::google::protobuf::Empty response;

        stubPtr_->SetMonitorInfo(nullptr, monito_info, &response, nullptr);

        LOG_INFO("Successfully set monitor info.")
    }
    void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo *monito_info)
    {

        ::google::protobuf::Empty request;
        stubPtr_->GetMonitorInfo(NULL, &request, monito_info, NewCallback(this, &RpcClient::closure_, response));

        LOG_INFO("Successfully get monitor info.")
    }

    void RpcClient::onConnection_(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            channel_->setConnection(conn);
        }
        else
        {
            RpcClient::connect();
        }
    }
    void RpcClient::closure_(::monitor::proto::MonitorInfo *resp)
    {
        ;
    }
}
