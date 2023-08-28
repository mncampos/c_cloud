#include "../headers/ServerSocket.hpp"
#include <netdb.h>
#include <arpa/inet.h>

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
    this->backupSockets.insert(std::make_pair(ip, socket));
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

bool ServerSocket::sendIP(std::string ip, int socket)
{
    Packet ipPkt = Packet(IP_PKT, 1, 1, ip.length() + 1, ip.c_str());
    return sendMessage(socket, std::move(ipPkt));
}

void ServerSocket::setIP(std::string IP)
{
    this->ip = IP;
}

std::string ServerSocket::getIP()
{
    return this->ip;
}

std::string ServerSocket::findIP()
{
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        std::cerr << "Erro ao obter o nome do host." << std::endl;
        return "";
    }

    hostent *host = gethostbyname(hostname);
    if (host == nullptr)
    {
        std::cerr << "Erro ao obter informações do host." << std::endl;
        return "";
    }

    in_addr *address = reinterpret_cast<in_addr *>(host->h_addr);
    return inet_ntoa(*address);
}

std::unordered_map<std::string, int> ServerSocket::getBackupSockets()
{
    return this->backupSockets;
}