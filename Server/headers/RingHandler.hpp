#ifndef RINGHANDLER_HPP
#define RINGHANDLER_HPP

#include <unistd.h>
#include "./ServerSocket.hpp"
#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include "../../Common/FileHandler.hpp"
#include <numeric>

class RingHandler
{
public: 
    ServerSocket *serverSocket;

    RingHandler(int ringServerSocket, ServerSocket *serverSocket);

    int getRingSocket();

    std::string electionIp;

private:
    int ringSocket;
};

#endif