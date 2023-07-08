#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <unistd.h>
#include "./ServerSocket.hpp"
#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include "../../Common/FileHandler.hpp"
#include <numeric>

class ClientHandler
{
public:
    ServerSocket *serverSocket;

    ClientHandler(int clientSocket, ServerSocket *serverSocket);
    void handleClient();
    int getClientSocket();
    void setClientUsername(std::string username);
    std::string getClientUsername();

    void getSyncDir();
    void listServer(std::string username);

private:
    int clientSocket;
    std::string clientUsername;
};

#endif