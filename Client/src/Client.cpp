#include "../headers/Client.hpp"

Client::Client(const std::string &username, const std::string &serverIP, int port)
{
    this->username = username;
    this->port = port;
    this->serverIP = serverIP;
    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    this->connected = false;
}

bool Client::serverConnect()
{
    if (clientSocket == -1)
    {
        std::cerr << "Failed to create client socket!";
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->port);
    if (inet_pton(AF_INET, this->serverIP.c_str(), &(serverAddress.sin_addr)) <= 0)
    {
        std::cerr << "Failed to set server address!" << std::endl;
        close(clientSocket);
        return false;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to connect to server!" << std::endl;
        close(clientSocket);
        return false;
    }

    std::cout << "Connect to server successfully!" << std::endl;
    connected = true;
    return true;
}

void Client::communicate()
{
    while (this->connected == true)
    {
        std::string command;
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "exit")
        {
            this->connected = false;
        }
    }
}

int Client::getClientSocket()
{
    return this->clientSocket;
}

bool Client::getConnectionStatus()
{
    return this->connected;
}