#include "../headers/ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket, ServerSocket* serverSocket)
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