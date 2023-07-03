#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "../Common/Packet.hpp"

class Socket
{
public:
    Socket();
    virtual ~Socket();
    bool create();
    bool send(const Packet &pkt);
    bool receive(Packet &pkt);
    bool close();
    int getSocketFd();

protected:
    int socketFd;
};

#endif