#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <iostream>

#define BUF_SIZE 1024

class ISocket
{
public:
    virtual bool sendMessage(const char *message) = 0;
    virtual std::string receiveMessage() = 0;
    virtual ~ISocket(){};
};
