#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <unistd.h>
#include "./ServerSocket.hpp"
#include <iostream>

class ClientHandler
{
public:
    ClientHandler(int clientSocket, ServerSocket* serverSocket);
    void handleClient();
    int getClientSocket();
    void setClientUsername(std::string username);
    std::string getClientUsername();


    ServerSocket* serverSocket;



private:
    int clientSocket;
    std::string clientUsername;
};

#endif