#include <iostream>
#include "../headers/Server.hpp"
#include "../headers/BackupHandler.hpp"

BackupHandler::BackupHandler(int backupSocket, ServerSocket *serverSocket)
{
    this->backupSocket = backupSocket;
    this->serverSocket = serverSocket;
}

// void *heartbeatCheck(void *arg) //@TODO: apagar se nao usar thread abaixo
// {
//     Server *server = reinterpret_cast<Server *>(arg);

//     while (true)
//     {
//         Packet receivedPacket = server->serverSocket.receiveMessage(server->backupSocket.getSocketFd());
//         if (receivedPacket.type == HEARTBEAT)
//         {
//             std::cout << "[+] RESPONDAAAAA." << std::endl;
//         }
//         sleep(5);
//     }
// }

void BackupHandler::handleBackup()
{

    while (true)
    {

        // @TODO: atualizar replicas
        // fazer uma funcao q verifica se o sinal esta ativo e atualiza todas replicas
        // o conteudo é tudo que esta no servidor principal

        // @TODO: precisa criar essa thread para heartbeat?
        // pthread_t heartbeatCheckThread;
        // if (pthread_create(&heartbeatCheckThread, nullptr, heartbeatCheck, reinterpret_cast<void *>(this)) != 0)
        // {
        //     std::cerr << "[-] Heartbeat Check thread creation fail!" << std::endl;
        // }

        Packet pkt = this->serverSocket->receiveMessage(this->backupSocket);
        if (pkt.type == HEARTBEAT)
        {
            std::cout << "[-] Receive Heartbeat from backup and responding it!" << std::endl;
            this->serverSocket->sendMessage(this->backupSocket, Packet(HEARTBEAT));
        }
        sleep(5);
    }
}