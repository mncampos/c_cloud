#include "../headers/BackupHandler.hpp"

BackupHandler::BackupHandler(int backupServerSocket, ServerSocket *serverSocket)
{
    this->backupSocket = backupSocket;
    this->serverSocket = serverSocket;
}

void BackupHandler::handleBackup()
{

    while(true)
    {
        // Update backups 
    }
}