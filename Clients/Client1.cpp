#include "../ISocket.hpp"

class Client1 : ISocket
{
public:
int sendSocket;
int receiveSocket;
    bool sendMessage(const char *message)
    {
        struct sockaddr_in serverAddr;
        socklen_t addrSize;

        sendSocket = socket(PF_INET, SOCK_STREAM, 0);
        if (sendSocket < 0)
        {
            return false;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(7500);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        std::memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
        addrSize = sizeof(serverAddr);

        //std::cout << "Enter in thread..." << std::endl;

        if (connect(sendSocket, (struct sockaddr *)&serverAddr, addrSize) < 0)
        {
            std::cout << "Error with connection" << std::endl;
            return false;
        }

        if (send(sendSocket, message, std::strlen(message), 0) < 0)
        {
            std::cout << "Error with sending" << std::endl;
            return false;
        }

        std::cout << "Data sent" << std::endl;
        //pthread_exit(NULL);

        return true;
    }

    std::string receiveMessage()
    {
        char buffer[BUF_SIZE];
        char error[] = "ERROR with ";
        struct sockaddr_in serverAddr;
        socklen_t addrSize;

        receiveSocket = socket(PF_INET, SOCK_STREAM, 0);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(7500);
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
        addrSize = sizeof(serverAddr);

        if (connect(receiveSocket, (struct sockaddr *)&serverAddr, addrSize) < 0)
        {
            std::cout << "Error with connect " << errno << std::endl;
            return " ";
        }

        if (recv(receiveSocket, buffer, BUF_SIZE, 0) < 0)
        {
            std::cout << "Error with receive " << errno << std::endl;
            return " ";
        }

        std::cout << "Message successfully received!" << std::endl;
        return std::string(buffer);
    }
    ~Client1()
    {
        close(sendSocket);
        close(receiveSocket);
        std::cout << "Socket closed!" << std::endl;
    }
};

int main()
{
    int clientSocket;
    std::cout << "Write your message" << std::endl;
    std::string message;
    std::getline(std::cin, message);

    Client1 client;
    //clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    client.sendSocket = clientSocket;
    if (client.sendMessage(message.c_str()) == true)
    {
        std::cout << "All good!" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Something went wrong!" << std::endl;
        return -1;
    }

    return 0;
}