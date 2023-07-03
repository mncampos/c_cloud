#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "../../Common/Socket.hpp"

class ClientSocket : public Socket
{
public:
    ClientSocket(const std::string &serverAddress, int port);
    bool connectToServer();
    int getPort();
    std::string getAddress();

private:
    int serverSocketFd;
    std::string address;
    int port;
};

#endif