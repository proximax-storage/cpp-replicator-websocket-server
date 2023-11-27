#include <WSCHATNetwork/TCPClient.h>

namespace WSCHAT
{
    TCPClient::TCPClient(const std::string &address, int port) : _socket(_ioc)
    {
        tcp::resolver resolver{_ioc};
        _endpoints = resolver.resolve(address, std::to_string(port));
    }

    void TCPClient::Run()
    {
        net::async_connect(_socket, _endpoints, [this](boost::system::error_code ec, tcp::endpoint ep)
                           {
            if (!ec)
            {
                AsyncRead();
            } });
        _ioc.run();
    }

    void TCPClient::Stop()
    {
        boost::system::error_code ec;
        _socket.close(ec);
    }

    void TCPClient::SendMessage(const std::string &recipient, const std::string &message)
    {
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(ParseMessageToJson(recipient, message));
        // _outgoingMessages.push(message);

        if (queueIdle)
        {
            AsyncWrite();
        }
    }

    void TCPClient::AsyncRead()
    {
        net::async_read_until(_socket, _buffer, "\n", [this](boost::system::error_code ec, size_t bytesTransferred)
                              { OnRead(ec, bytesTransferred); });
    }

    void TCPClient::OnRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            Stop();
            return;
        }

        std::stringstream message;
        message << std::istream{&_buffer}.rdbuf();
        OnMessage(message.str());
        AsyncRead();
    }

    void TCPClient::AsyncWrite()
    {
        net::async_write(_socket, net::buffer(_outgoingMessages.front()), [this](boost::system::error_code ec, size_t bytesTransferred)
                         { OnWrite(ec, bytesTransferred); });
    }

    void TCPClient::OnWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            Stop();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty())
        {
            AsyncWrite();
        }
    }

    std::string TCPClient::ParseMessageToJson(const std::string &recipient, const std::string &message)
    {
        pt::ptree propertyTree;
        pt::ptree messageChild;
        std::ostringstream buffer;

        messageChild.put("file", "demo.txt");
        messageChild.put("content", message);

        propertyTree.put("sender", "client");
        propertyTree.add("recipient", recipient);
        propertyTree.add_child("message", messageChild);
        propertyTree.add("timestamp", GetCurrentDateTime());

        pt::write_json(buffer, propertyTree, false);
        return buffer.str();
    }

    char *TCPClient::GetCurrentDateTime()
    {
        time_t now = time(0);
        return ctime(&now);
    }
}