#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "../../Common/Socket.hpp"
#include <fstream>
#include <cmath>

class ClientSocket : public Socket
{
public:
    ClientSocket(const std::string &serverAddress, int port);
    bool connectToServer();
    int getPort();
    bool sendUsername(std::string username);
    std::string getAddress();
    bool getSyncFile(std::string filename, std::string username);
    

private:
    int serverSocketFd;
    std::string address;
    int port;
};

#endif