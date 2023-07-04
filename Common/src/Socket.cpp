#include "../Socket.hpp"

Socket::Socket() : socketFd(-1) {}

Socket::~Socket()
{
    close();
}

bool Socket::create()
{
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    return socketFd != -1;
}

bool Socket::close()
{
    if (socketFd != -1)
    {
        if (::close(socketFd) == 0)
        {
            socketFd = -1;
            return true;
        }
    }
    return false;
}

int Socket::getSocketFd()
{
    return this->socketFd;
}