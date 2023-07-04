#include "../headers/ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket, ServerSocket *serverSocket)
{
    this->clientSocket = clientSocket;
    this->serverSocket = serverSocket;
}

int ClientHandler::handleClient()
{
    Packet pkt = this->serverSocket->receiveData();
    if (pkt.type == COMMAND_PKT)
    {
        std::cout << "Received command " << pkt.payload.get() << " from user: " << this->clientUsername << std::endl;

        if (strcmp(pkt.payload.get(), "exit") == 0)
        {
            std::cout << "Disconnecting " << this->clientUsername << std::endl;
            return -1;
        }

        return -1;
    }

    return -1;
}

int ClientHandler::getClientSocket()
{
    return this->clientSocket;
}

void ClientHandler::setClientUsername(std::string username)
{
    this->clientUsername = username;
}

std::string ClientHandler::getClientUsername()
{
    return this->clientUsername;
}

void ClientHandler::getSyncDir()
{
    std::string syncDir = "sync_dir_" + clientUsername;

    struct stat dir;

    if (stat(syncDir.c_str(), &dir) == -1) // If the directory doesn't exists
    {
        if (mkdir(syncDir.c_str(), 0777) == 0)
        {
            std::cout << "[+] Sync dir created for " << clientUsername << " - " << syncDir << std::endl;
        }
        else
            std::cout << "[-] Failed to create sync dir!" << std::endl;
    }
    else
        std::cout << "[#] " << syncDir << " found." << std::endl;
}