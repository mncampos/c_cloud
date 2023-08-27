#include "../headers/RingHandler.hpp"

RingHandler::RingHandler(int ringServerSocket, ServerSocket *serverSocket)
{
    this->ringSocket = ringServerSocket;
    this->serverSocket = serverSocket;
}

int RingHandler::getRingSocket()
{
    return this->ringSocket;
}