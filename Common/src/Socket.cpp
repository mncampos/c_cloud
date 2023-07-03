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

bool Socket::send(const Packet &pkt)
{
    ssize_t bytesSent = ::send(socketFd, &pkt, sizeof(pkt), 0);
    return bytesSent == sizeof(pkt);
}

bool Socket::receive(Packet &pkt)
{
    ssize_t bytesRead = ::recv(socketFd, &pkt, sizeof(pkt), 0);
    return bytesRead == sizeof(pkt);
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