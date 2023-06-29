#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ClientHandler.hpp"

const int PORT = 4000;

class Server
{
public:
    Server();
    bool start();
    void run();

private:
    int serverSocket;
    sockaddr_in serverAddress;
};

#endif