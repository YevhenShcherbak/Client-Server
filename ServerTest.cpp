#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#define BUF_SIZE 1024

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void socketThread(int clientSocket)
{
    char client_message[2000];
    char buffer[BUF_SIZE];

    int newSocket = clientSocket;

    pthread_mutex_lock(&lock);
    char *message = (char *)malloc(sizeof(client_message) + 20);
    strcpy(message, "Hello Client : ");
    strcat(message, client_message);
    strcat(message, "\n");
    strcpy(buffer, message);
    free(message);
    pthread_mutex_unlock(&lock);
    sleep(1);
    send(newSocket, buffer, 20, 0);
    std::cout << "End of thread" << std::endl;
    close(newSocket);
}

int main()
{
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    pid_t pid[50];

    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7800);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (listen(serverSocket, 50) == 0)
    {
        std::cout << "Listening..." << std::endl;
    }
    else
    {
        perror("listen");
    }
    pthread_t tid[60];
    int i = 0;
    while (true)
    {
        addr_size = sizeof(serverStorage);
        newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size);
        int pid_c = 0;

        if ((pid_c = fork()) == 0)
        {
            socketThread(newSocket);
        }
        else
        {
            pid[i++] = pid_c;
            if (i >= 49)
            {
                i = 0;
                while (i < 50)
                    waitpid(pid[i++], NULL, 0);
                i = 0;
            }
        }
    }
    return 0;
}