#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include "../../Common/Socket.hpp"
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class ServerSocket : public Socket
{
public:
    ServerSocket(int port);
    bool bind();
    bool listen(int backlog = 5);
    int accept();
    bool connectBackupToServer(std::string serverIp, int port);

    void addClientSocket(std::string username, int socket);
    void addBackupSocket(std::string ip, int socket);
    void sendSignal(std::string username, int signalCode, int excludedSocket); // See packet.hpp for codes
    void removeClientSocket(int socket);

    bool sendIP(std::string ip, int socket);
    void setIP(std::string IP);
    std::string getIP();
    std::string findIP();

    std::unordered_map<std::string, int> getBackupSockets();

private:
    int port;
    std::string ip;
    std::unordered_map<std::string, std::vector<int>> clientSockets;
    std::unordered_map<std::string, int> backupSockets;
};

#endif