#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <unistd.h>
#include "./ServerSocket.hpp"
#include <iostream>
#include <sys/stat.h>

class ClientHandler
{
public:
    ServerSocket *serverSocket;

    ClientHandler(int clientSocket, ServerSocket *serverSocket);
    int handleClient();
    int getClientSocket();
    void setClientUsername(std::string username);
    std::string getClientUsername();

    void getSyncDir();

private:
    int clientSocket;
    std::string clientUsername;
};

#endif