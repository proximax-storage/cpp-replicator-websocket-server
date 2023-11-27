#pragma once

#include <WSCHATNetwork/TCPConnection.h>

#include <boost/asio.hpp>

#include <string>
#include <optional>
#include <unordered_set>

namespace WSCHAT
{
    namespace net = boost::asio;
    using tcp = net::ip::tcp;

    enum class IPVersion
    {
        V4,
        V6
    };

    class TCPServer
    {
        using OnJoinHandler = std::function<void(TCPConnection::ptr)>;
        using OnLeaveHandler = std::function<void(TCPConnection::ptr)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        TCPServer(IPVersion ipVersion, int port);

        int Run();

        void Broadcast(const std::string &message);

        void DirectMessage(TCPConnection::ptr _connection, const std::string &message);

        std::unordered_set<TCPConnection::ptr> GetConnectedClients();

    private:
        void StartAccept();

    public:
        OnJoinHandler OnJoin;
        OnLeaveHandler OnLeave;
        OnClientMessageHandler OnClientMessage;

    private:
        IPVersion _ipVersion;
        int _port;

        net::io_context _ioc;
        tcp::acceptor _acceptor;
        std::optional<tcp::socket> _socket;
        std::unordered_set<TCPConnection::ptr> _connections{};
    };
}