#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <unistd.h>
#include <iostream>
#include "../../Common/header/Packet.hpp"

class ClientHandler
{
public:
    ClientHandler(int clientSocket);
    void handleClient();
    int getClientSocket();

private:
    int clientSocket;
};

#endif