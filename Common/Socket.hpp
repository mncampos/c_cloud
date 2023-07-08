#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include "../Common/Packet.hpp"

class Socket
{
public:
    Socket();
    virtual ~Socket();
    bool create();
    bool close();
    int getSocketFd();

    bool sendFile(std::string filename, int clientSocketFd);
    Packet receiveMessage(int clientSocketFd);
    bool sendMessage(int socketFd, Packet messagePacket);
    bool receiveFile(std::string filename, int clientSocketFd, std::string username);

protected:
    int socketFd;
};

#endif