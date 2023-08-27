#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ClientHandler.hpp"
#include "BackupHandler.hpp"
#include "./ServerSocket.hpp"

const int PORT = 4000;
const int CONNECTIONS_LIMIT = 3;

const int BACKUP_PORT = 5000;
const int BACKUP_CONNECTIONS_LIMIT = 2;

const int RING_PORT = 6000;
const int RING_CONNECTIONS_LIMIT = 2;

class Server
{
public:
    Server();
    void run();
    void runBackup(std::string mainServerIp);
    void runElection(std::string msgIp);

    ServerSocket serverSocket;
    ServerSocket backupSocket;
    ServerSocket ringSocket;

private:
};

#endif