#include "../headers/ClientSocket.hpp"

ClientSocket::ClientSocket(const std::string &serverAddress, int port)
    : Socket(), address(serverAddress), port(port)
{
    if (!create())
    {
        std::cerr << "Error opening socket!" << std::endl;
    }
}

bool ClientSocket::connectToServer()
{
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, address.c_str(), &(serverAddress.sin_addr)) <= 0)
    {
        std::cerr << "Failed to set server address!" << std::endl;
        return false;
    }

    std::cout << "Attempting to connect to server " << address << ":" << port <<  "..." << std::endl;

    return ::connect(socketFd, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) != -1;
}

int ClientSocket::getPort()
{
    return this->port;
}

std::string ClientSocket::getAddress()
{
    return this->address;
}

bool ClientSocket::sendUsername(std::string username)
{
    Packet usernamePkt = Packet(USERNAME_PKT, 1, 1, username.length() + 1, username.c_str());
    std::vector<uint8_t> serializedPacket = usernamePkt.serialize();

    return ::send(socketFd, serializedPacket.data(), serializedPacket.size(), 0) > 0;


}