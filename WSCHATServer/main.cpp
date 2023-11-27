#include <WSCHATNetwork/TCPServer.h>

#include <iostream>

int main(int agrc, char *argv[])
{
    WSCHAT::TCPServer server{WSCHAT::IPVersion::V4, 8083};

    server.OnJoin = [](WSCHAT::TCPConnection::ptr server)
    {
        std::cout << "Client has joined the server: " << server->GetUsername() << std::endl;
    };

    server.OnLeave = [](WSCHAT::TCPConnection::ptr server)
    {
        std::cout << "Client has left the server: " << server->GetUsername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string &message)
    {
        // server.Broadcast(message);
        // std::string recipient = server.GetConnectedClients().begin()->get()->GetUsername();
        // std::cout << server.GetConnectedClients().begin()->get()->GetUsername() << std::endl;

        // for (auto &client : server.GetConnectedClients())
        // {
        //     if (client.get()->GetUsername() + "\n" == message)
        //     {
        //         server.DirectMessage(client.get()->shared_from_this(), message);
        //         break;
        //     }
        // }

        std::cout << message[message.find_last_of("recipient")] << std::endl;
    };

    server.Run();
    return 0;
}