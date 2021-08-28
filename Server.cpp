#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "Shm.hpp"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
char clientMessage[BUF_SIZE + 1];

void socketThread(int clientSocket)
{
    char clientMessage[BUF_SIZE];
    char buf[BUF_SIZE];

    int newSocket = clientSocket;
    
    pthread_mutex_lock(&lock);
    char *message = static_cast<char *>(malloc(sizeof(clientMessage) + 8));
    std::strcpy(message,"Client> ");
    std::strcat(message, clientMessage);
    std::strcat(message, "\n");
    std::strcpy(buf, message);
    free(message);
    pthread_mutex_unlock(&lock);
    sleep(1);
    send(newSocket, buf, sizeof(buf), 0);
    close(newSocket);
} 
//TODO: add multithreading
int main()
{
    int sock, listener;
    struct sockaddr_in serverAddr;
    char buf[BUF_SIZE];
    int bytes_read;
    bool flag = true;
    pid_t pid[50];
    //Shm shm;

    listener = socket(PF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        exit(1);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(3426);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(listener, 50);

    while (flag)
    {
        sock = accept(listener, NULL, NULL);
        if (sock < 0)
        {
            perror("accept");
            exit(2);
        }

        while (flag)
        {
            bytes_read = recv(sock, buf, BUF_SIZE, 0);
            if (bytes_read <= 0)
            {
                break;
            }
            std::cout << "Client> " << buf << std::endl;
            send(sock, buf, BUF_SIZE, 0);
        }
        close(sock);
    }

    return 0;
}
