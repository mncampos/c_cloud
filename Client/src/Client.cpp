#include "../headers/Client.hpp"

Client::Client(const std::string &username, const std::string &serverIP, int port)
    : username(username), socket(serverIP, port)
{
}
void Client::uploadFile(std::string fileName)
{
    std::string command = "upload";

    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(COMMAND_PKT, 1, 1, command.length() + 1, command.c_str())))
        std::cerr << "Error sending command!" << std::endl;

    sleep(1);

    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(FILENAME_PKT, 1, 1, fileName.length() + 1, fileName.c_str())))
        std::cerr << "Error sending file name!" << std::endl;

    sleep(1);

    this->socket.sendFile(fileName, this->socket.getSocketFd());
}

void Client::downloadFile(std::string fileName)
{
}

void Client::deleteFile(std::string fileName)
{
}
void Client::listServerFiles()
{
}
void Client::listClientFiles()
{
}
void Client::getSyncDir()
{
}