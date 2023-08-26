#include "../headers/BackupHandler.hpp"

BackupHandler::BackupHandler(int backupSocket, ServerSocket *serverSocket)
{
    this->backupSocket = backupSocket;
    this->serverSocket = serverSocket;
}

void BackupHandler::handleBackup()
{

    while (true)
    {

        

        // Packet pkt = serverSocket->receiveMessage(backupSocket);
        // Update backups

        // switch (pkt.type)
        // {
        // case UPDATE:
        //     break;
        // case ELECTION:
        //     // verifica se o id atual é maior do que o que recebeu ou igual
        //     //  se for igual, é o novo eleito
        //     //  se for maior, passa pra frente
        //     break;
        // default:
        //     continue;
        // }
    }
}