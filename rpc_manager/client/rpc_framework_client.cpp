#include "rpc_framework_client.h"

namespace monitor
{
    RpcClient::RpcClient(network::EventLoop *loop, const network::InetAddress &serverAddr)
        : loop_(loop),
          clientPtr_(new network::TcpClient(loop, serverAddr, "RpcClient")),
          channel_(new network::RpcChannel),
          stub_(network::get_pointer(channel_))

    {
        // 初始化日志
        Log::Instance()->init(1, "./client_log", ".log", 1024);
        LOG_INFO("<-----------------CLIENT---------------->");

        clientPtr_->setConnectionCallback(std::bind(&RpcClient::onConnection_, this, std::placeholders::_1));
        clientPtr_->setMessageCallback(std::bind(&network::RpcChannel::onMessage, network::get_pointer(channel_), std::placeholders::_1, std::placeholders::_2));
    }

    RpcClient::RpcClient(network::EventLoop *loop)
        : loop_(loop),
          channel_(new network::RpcChannel),
          stub_(network::get_pointer(channel_))

    {

        // 初始化日志
        Log::Instance()->init(1, "./client_log", ".log", 1024);
        LOG_INFO("<-----------------CLIENT---------------->");

        std::ifstream ifs("ip_address.txt");
        std::string line;
        std::getline(ifs, line);

        std::string ip;
        uint16_t port;
        if (ifs.eof() || line.empty())
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
                ip = line.substr(0, pos);
                port = std::stoi(line.substr(pos + 1, line.size() - pos - 1));
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

        clientPtr_.reset(new network::TcpClient(loop, serverAddr, "RpcClient"));

        clientPtr_->setConnectionCallback(std::bind(&RpcClient::onConnection_, this, std::placeholders::_1));
        clientPtr_->setMessageCallback(std::bind(&network::RpcChannel::onMessage, get_pointer(channel_), std::placeholders::_1, std::placeholders::_2));
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
    void RpcClient::SetMonitorInfo(const monitor::proto::MonitorInfo *monito_info)
    {

        ::google::protobuf::Empty response;

        stub_.SetMonitorInfo(NULL, monito_info, &response, ::google::protobuf::NewCallback(this, &RpcClient::closure_));

        LOG_INFO("Successfully set monitor info.");
    }
    void RpcClient::GetMonitorInfo(monitor::proto::MonitorInfo *monito_info)
    {

        ::google::protobuf::Empty request;

        stub_.GetMonitorInfo(NULL, &request, monito_info, ::google::protobuf::NewCallback(this, &RpcClient::closure_));

        LOG_INFO("Successfully get monitor info.");
    }

    void RpcClient::onConnection_(const network::TcpConnectionPtr &conn)
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
    void RpcClient::closure_()
    {
        ;
    }
}
