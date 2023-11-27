#include <WSCHATNetwork/TCPServer.h>

#include <boost/asio.hpp>

#include <iostream>

namespace WSCHAT
{
    TCPServer::TCPServer(IPVersion ipVersion, int port)
        : _ipVersion(ipVersion),
          _port(port),
          _acceptor(_ioc, tcp::endpoint(ipVersion == IPVersion::V4 ? tcp::v4() : tcp::v6(), _port))
    {
    }

    int TCPServer::Run()
    {
        try
        {
            StartAccept();
            _ioc.run();
        }
        catch (std::exception e)
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }
        return 0;
    }

    void TCPServer::Broadcast(const std::string &message)
    {
        for (auto &connection : _connections)
        {
            connection->SendMessage(message);
        }
    }

    void TCPServer::DirectMessage(TCPConnection::ptr connection, const std::string &message)
    {
        connection->SendMessage(message);
    }

    std::unordered_set<TCPConnection::ptr> TCPServer::GetConnectedClients()
    {
        return _connections;
    }

    void TCPServer::StartAccept()
    {
        _socket.emplace(_ioc);

        // Async accept the connection
        _acceptor.async_accept(*_socket, [this](const boost::system::error_code &ec)
                               {
            // Create a connection
            auto connection = TCPConnection::Create(std::move(*_socket));

            if (OnJoin)
            {
                OnJoin(connection);
            }

            // Accepts multiple clients connection
            _connections.insert(connection);

            if (!ec) 
            {
                connection->Start(
                    [this](const std::string &message)
                    {
                        if (OnClientMessage)
                        {
                            OnClientMessage(message);
                        }
                    }, 
                    [&, weakPtr = std::weak_ptr(connection)]
                    {
                    if (auto sharedPtr = weakPtr.lock(); sharedPtr && _connections.erase(sharedPtr)){
                        if (OnLeave)
                        {
                            OnLeave(sharedPtr);
                        }
                    }
                    });
            }
            StartAccept(); });
    }
}
