#include "../headers/ServerSocket.hpp"

ServerSocket::ServerSocket(int port) : port(port)
{
    if (!create()){
        std::cout << "[-] Error creating server socket!" << std::endl;
        exit(-1);
    }
    if (!bind()){
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

bool ServerSocket::accept()
{
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);

    clientSocketFd = ::accept(socketFd, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressSize);

    if (clientSocketFd != -1)
        return true;
    else
        return false;
}

int ServerSocket::getClientSocketFd()
{
    return this->clientSocketFd;
}

Packet ServerSocket::receiveData()
{
    std::vector<uint8_t> dataBuffer(MAX_PAYLOAD);
    ssize_t bytesRead = recv(clientSocketFd, dataBuffer.data(), dataBuffer.size(), 0);
    if (bytesRead == -1) {
        std::cerr << "[-] Failed to receive data" << std::endl;
        return Packet(FAILURE);
    }
    if (bytesRead == 0) {
        std::cerr << "[-] Connection closed" << std::endl;
        ::close(clientSocketFd);
        return Packet(FAILURE);
    }
    std::vector<uint8_t> receivedData(dataBuffer.begin(), dataBuffer.begin() + bytesRead);
    Packet receivedPacket = Packet::deserialize(receivedData);

    if(bytesRead < receivedPacket.length){
        std::cerr << "[-] Received incomplete packet!" << std::endl;
        return Packet(FAILURE);
    }

    return receivedPacket;
}