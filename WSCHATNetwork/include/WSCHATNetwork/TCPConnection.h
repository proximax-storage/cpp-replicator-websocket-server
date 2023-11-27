#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <queue>

namespace WSCHAT
{
    namespace net = boost::asio;
    using tcp = net::ip::tcp;

    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

    class TCPConnection : public std::enable_shared_from_this<TCPConnection>
    {
    public:
        using ptr = std::shared_ptr<TCPConnection>;

        static ptr Create(tcp::socket &&socket)
        {
            return ptr(new TCPConnection(std::move(socket)));
        }

        void Start(MessageHandler &&messageHandler, ErrorHandler &&errorHandler);

        void SendMessage(const std::string &message);

        tcp::socket &GetSocket()
        {
            return _socket;
        }

        inline const std::string &GetUsername() const
        {
            return _username;
        }

    private:
        explicit TCPConnection(tcp::socket &&socket);

        void AsyncRead();

        void OnRead(boost::system::error_code ec, size_t bytesTransferred);

        void AsyncWrite();

        void OnWrite(boost::system::error_code ec, size_t bytesTransferred);

    private:
        tcp::socket _socket;
        std::string _username;

        std::queue<std::string> _outgoingMessages;
        net::streambuf _buffer{65536};

        MessageHandler _messageHandler;
        ErrorHandler _errorHandler;
    };
}