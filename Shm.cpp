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
    char* addr;
public:
    int shm;
    int mode = O_CREAT;
    Shm();
    ~Shm();
    bool shm_write(char* message);
    const char* shm_read();
};

Shm::Shm()
{
    if ((shm = shm_open(MEMORY_NAME, mode | O_RDWR, 0777)) == -1)
    {
        perror("shared memory open");
        exit(1);
    }
    std::cout << "Shared memory was created" << std::endl;
}

Shm::~Shm()
{
    shm_unlink(MEMORY_NAME);
    std::cout << "Shared memory was unlinked" << std::endl;
}

bool Shm::shm_write(char* message)
{
    if (ftruncate(shm, BUF_SIZE + 1)==-1)
    {
        perror("ftruncate");
        exit(1);
    }

    addr = (char *)mmap(0, BUF_SIZE+1, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    if (addr == (char *)-1)
    {
        perror("mmap");
        exit(1);
    }
    memcpy(addr, message, BUF_SIZE);    
    std::cout << "Shared memory was writed." << std::endl;
    munmap(addr, BUF_SIZE);
    close(shm);
    
    shm_unlink(MEMORY_NAME);
    return true;
}

const char* Shm::shm_read()
{
    char *receive;
    if ((shm = shm_open(MEMORY_NAME, mode | O_RDWR, 0777)) == -1)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(shm, BUF_SIZE + 1) == -1)
    {
        perror("ftruncate");
        exit(2);
    }

    addr = (char*)mmap(0, BUF_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    if (addr = (char*)-1)
    {
        perror("mmap");
        exit(3);
    }
    receive = addr;
    munmap(addr, BUF_SIZE);
    close(shm);
    shm_unlink(MEMORY_NAME);

    return receive;
}