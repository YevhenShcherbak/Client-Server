#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include "Shm.hpp"

char message[] = "Hello, you on the other side, mate!\n";
char buf[sizeof(message)];

int main()
{
    int sock;
    struct sockaddr_in addr;
    //Shm shm;
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3426);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (connect(sock, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    //shm.shm_write((char *)message);
    //std::cout << "Server> " << shm.shm_read() << std::endl;
    send(sock, message, sizeof(message), 0);
    recv(sock, buf, sizeof(message), 0);
    std::cout << "Server> " << buf << std::endl;
    close(sock);

    return 0;
}