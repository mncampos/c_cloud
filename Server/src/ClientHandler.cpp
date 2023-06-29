#include "../headers/ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket)
{
    this->clientSocket = clientSocket;
}

void ClientHandler::handleClient()
{
    std::cout << "Im handling the client " << this->clientSocket << std::endl;
}

int ClientHandler::getClientSocket()
{
    return this->clientSocket;
}