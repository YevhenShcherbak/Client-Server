#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

#define BUF_SIZE 4096

int main()
{
	int sock, listener;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];
    int bytes_read;
    bool flag = true;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3426);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }
    
    listen(listener, 1);

    while(flag)
    {
        sock = accept(listener, NULL, NULL);
        if (sock<0)
        {
            perror("accept");
            exit(2);
        }

        while (flag)
        {
            bytes_read = recv(sock, buf, BUF_SIZE, 0);
            if (bytes_read <= 0)
            {
                flag = false;
            } 
            std::cout << "Client> " << buf << std::endl;
            send(sock, buf, BUF_SIZE, 0);
        }
        close(sock);
    }

    return 0;
}
