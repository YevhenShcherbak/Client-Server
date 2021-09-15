#include <iostream>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 4096
#define MEMORY_NAME "Shm Client-Server"

class Shm
{
private:
    char *addr;

public:
    int shm;
    int mode = O_CREAT;
    Shm(std::string memoryName);
    ~Shm();
    bool shm_write(char *message);
    const char *shm_read();
};
