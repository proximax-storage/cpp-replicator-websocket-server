#include <WSCHATNetwork/TCPConnection.h>

#include <iostream>
#include <memory>

namespace WSCHAT
{
    TCPConnection::TCPConnection(tcp::socket &&socket)
        : _socket(std::move(socket))
    {
        boost::system::error_code ec;
        std::stringstream username;
        username << _socket.remote_endpoint();
        _username = username.str();
    }

    void TCPConnection::Start(MessageHandler &&messageHandler, ErrorHandler &&errorHandler)
    {
        _messageHandler = std::move(messageHandler);
        _errorHandler = std::move(errorHandler);
        AsyncRead();
    }

    void TCPConnection::SendMessage(const std::string &message)
    {
        bool queueIdle = _outgoingMessages.empty();
        _outgoingMessages.push(message);

        if (queueIdle)
        {
            AsyncWrite();
        }
    }

    void TCPConnection::AsyncRead()
    {
        net::async_read_until(_socket, _buffer, "\n", [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
                              { self->OnRead(ec, bytesTransferred); });
    }

    void TCPConnection::OnRead(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            _socket.close(ec);
            _errorHandler();
            return;
        }

        std::stringstream message;
        // message << _username << ": " << std::istream(&_buffer).rdbuf();
        message << std::istream(&_buffer).rdbuf();
        _buffer.consume(bytesTransferred);
        _messageHandler(message.str());
        AsyncRead();
    }

    void TCPConnection::AsyncWrite()
    {
        net::async_write(_socket, net::buffer(_outgoingMessages.front()), [self = shared_from_this()](boost::system::error_code ec, size_t bytesTransferred)
                         { self->OnWrite(ec, bytesTransferred); });
    }

    void TCPConnection::OnWrite(boost::system::error_code ec, size_t bytesTransferred)
    {
        if (ec)
        {
            _socket.close(ec);
            _errorHandler();
            return;
        }

        _outgoingMessages.pop();

        if (!_outgoingMessages.empty())
        {
            AsyncWrite();
        }
    }
}