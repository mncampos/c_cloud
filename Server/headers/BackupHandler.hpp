#ifndef BACKUPHANDLER_HPP
#define BACKUPHANDLER_HPP

#include <unistd.h>
#include "./ServerSocket.hpp"
#include <iostream>
#include <sys/stat.h>
#include <filesystem>
#include "../../Common/FileHandler.hpp"
#include <numeric>

class BackupHandler
{
public: 
    ServerSocket *serverSocket;

    BackupHandler(int backupServerSocket, ServerSocket *serverSocket);

    void handleBackup();
    int getBackupSocket();

private:
    int backupSocket;
};

#endif