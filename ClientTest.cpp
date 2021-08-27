#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <string.h>

#include <thread>

#define BUF_SIZE 1024

void *clientThread(void *arg)
{
    std::cout << "Enter in thread" << std::endl;
    char message[2000];
    char buffer[BUF_SIZE];
    int clientSock;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    clientSock = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    addr_size = sizeof(serverAddr);
    connect(clientSock, (struct sockaddr *)&serverAddr, addr_size);
    strcpy(message, "Hey");

    if (send(clientSock, message, strlen(message), 0) < 0)
    {
        perror("send");
    }

    if (recv(clientSock, buffer, BUF_SIZE, 0) < 0)
    {
        perror("receive");
    }

    std::cout << "Data received: " << buffer << std::endl;
    close(clientSock);
    pthread_exit(NULL);
}

int main()
{
    int i = 0;
    pthread_t tid[5];
    while (i < 5)
    {
        if (pthread_create(&tid[i], NULL, clientThread, NULL) != 0)
        {
            perror("thread create");
        }
        i++;
    }
    sleep(5);
    i = 0;
    while (i < 5)
    {
        pthread_join(tid[i++], NULL);
        std::cout << i << std::endl;
    }
}