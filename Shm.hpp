#include <sys/mman.h>
#include <fcntl.h>
#include <string>
#include <iostream>

#ifndef Shm.hpp
#define BUF_SIZE 4096
#define MEMORY_NAME "Shm Client-Server"

class Shm
{
public:
    Shm();
    ~Shm();
}

#endif