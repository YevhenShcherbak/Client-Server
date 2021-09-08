#include "ISocket.hpp"

class Server : ISocket
{
public:
    int sendSocket;
    int receiveSocket;
    bool sendMessage(const char *message)
    {
        struct sockaddr_in serverAddr;
        socklen_t addrSize;
        sendSocket = socket(PF_INET, SOCK_STREAM, 0);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(7500);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
        addrSize = sizeof(serverAddr);

        if (send(sendSocket, message, strlen(message), 0) < 0)
        {
            std::cout << "Error with sending" << std::endl;
            return false;
        }

        std::cout << "Data sent" << std::endl;

        return true;
    }
    std::string receiveMessage()
    {
        std::string message;
        char buffer[BUF_SIZE];
        if (recv(receiveSocket, buffer, BUF_SIZE, 0) < 0)
        {
            std::cout << "Error with receive " << errno << std::endl;
            return " ";
        }
        message = buffer;
        memset(buffer, '\0', sizeof(buffer));
        return message;
    }
    ~Server()
    {
        close(sendSocket);
        close(receiveSocket);
        std::cout << "Socket closed" << std::endl;
    }
};

int main()
{
    Server server;
    std::string message;
    int serverSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrSize;

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7500);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Error with binding " << errno << std::endl;
    }
    if (listen(serverSocket, 15) == 0)
    {
        std::cout << "Listening... " << std::endl;
    }
    else
    {
        std::cout << "Error with listen" << std::endl;
        exit(1);
    }
    while (true)
    {
        memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
        addrSize = sizeof(serverAddr);
        server.receiveSocket = accept(serverSocket, 0, 0);
        std::cout << "Client> " << server.receiveMessage() << std::endl;
    }
}