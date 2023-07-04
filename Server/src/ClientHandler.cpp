#include "../headers/ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket, ServerSocket *serverSocket)
{
    this->clientSocket = clientSocket;
    this->serverSocket = serverSocket;
}

void ClientHandler::handleClient()
{
    std::cout << "Handling client number " << this->clientSocket << std::endl;
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