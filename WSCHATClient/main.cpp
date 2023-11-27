#include <WSCHATNetwork/TCPClient.h>

#include <iostream>
#include <string>
#include <thread>

using namespace WSCHAT;

int main(int agrc, char *argv[])
{
    if (agrc != 3)
    {
        std::cout << "<WSCHATClient> <address> <port>" << std::endl;
        std::cout << "Example: ./WSCHATClient 127.0.0.1 8083" << std::endl;
        return 0;
    }

    std::string address(argv[1]);
    int port = atoi(argv[2]);

    TCPClient client{address, port};
    client.OnMessage = [](const std::string &message)
    { std::cout << message; };

    std::thread t{[&client]()
                  { client.Run(); }};

    while (true)
    {

        std::string recipient;
        std::cout << "send to: ";
        getline(std::cin, recipient);

        std::string message;
        std::cout << "message input: ";
        getline(std::cin, message);

        if (message == "\\q")
        {
            break;
        }
        message += "\n";
        client.SendMessage(recipient, message);
    }

    client.Stop();
    t.join();
    return 0;
}