#include "../headers/ServerSocket.hpp"

ServerSocket::ServerSocket(int port) : port(port)
{
    if (!create())
    {
        std::cout << "[-] Error creating server socket!" << std::endl;
        exit(-1);
    }
    if (!bind())
    {
        std::cout << "[-] Error binding server socket!" << std::endl;
        exit(-1);
    }
}

bool ServerSocket::bind()
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    return ::bind(socketFd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != -1;
}

bool ServerSocket::listen(int backlog)
{
    return ::listen(socketFd, backlog) != -1;
}

int ServerSocket::accept()
{
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);

    int clientSocketFd = ::accept(socketFd, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressSize);

    if (clientSocketFd != -1)
        return clientSocketFd;
    else
        return -1;
}

