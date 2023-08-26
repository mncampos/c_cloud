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

void ServerSocket::addClientSocket(std::string username, int socket)
{
    this->clientSockets[username].push_back(socket);
}

void ServerSocket::addBackupSocket(std::string ip, int socket)
{
    this->backupSockets[ip].push_back(socket);
}

void ServerSocket::sendSignal(std::string username, int signalCode, int excludedSocket)
{
    auto iterator = clientSockets.find(username);
    if (iterator != clientSockets.end())
    {
        const std::vector<int> &sockets = iterator->second;
        for (int socket : sockets)
        {

            if (socket != excludedSocket)
            {
                sendMessage(socket, Packet(signalCode));
            }
        }
    }
}

void ServerSocket::removeClientSocket(int socketFd)
{
    for (auto &pair : clientSockets)
    {
        std::vector<int> &sockets = pair.second;
        auto it = std::find(sockets.begin(), sockets.end(), socketFd);
        if (it != sockets.end())
        {
            sockets.erase(it);
            if (sockets.empty())
            {
                clientSockets.erase(pair.first);
            }
            break;
        }
    }
}

bool ServerSocket::connectBackupToServer(std::string serverIp, int port)
{
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, serverIp.c_str(), &(serverAddress.sin_addr)) <= 0)
    {
        std::cerr << "[-] Failed to set server address!" << std::endl;
        return false;
    }

    std::cout << "[#] Attempting to connect to server " << serverIp << ":" << port << "..." << std::endl;

    return ::connect(socketFd, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) != -1;
}
