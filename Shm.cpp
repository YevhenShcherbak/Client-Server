#include "Shm.hpp"

Shm::Shm(std::string memoryName)
{
    if ((this->shm = shm_open(memoryName.c_str(), mode | O_RDWR, 0777)) == -1)
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

bool Shm::shm_write(char *message)
{
    if (ftruncate(this->shm, BUF_SIZE + 1) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    addr = (char *)mmap(0, BUF_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, this->shm, 0);
    if (addr == (char *)-1)
    {
        perror("mmap");
        exit(1);
    }
    memcpy(addr, message, BUF_SIZE);
    std::cout << "Shared memory was wrote." << std::endl;
    munmap(addr, BUF_SIZE);
    close(this->shm);

    
    return true;
}

const char *Shm::shm_read()
{
    char *receive;
    if ((this->shm = shm_open(MEMORY_NAME, mode | O_RDWR, 0777)) == -1)
    {
        perror("shm_open");
        exit(1);
    }

    if (ftruncate(this->shm, BUF_SIZE + 1) == -1)
    {
        perror("ftruncate");
        exit(2);
    }

    addr = (char *)mmap(0, BUF_SIZE + 1, PROT_WRITE | PROT_READ, MAP_SHARED, this->shm, 0);
    if (addr = (char *)-1)
    {
        perror("mmap");
        exit(3);
    }
    receive = addr;
    munmap(addr, BUF_SIZE);
    close(this->shm);

    return receive;
}