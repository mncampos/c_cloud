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
        std::cerr << "[-] Error sending command!" << std::endl;

    sleep(1);

    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(FILENAME_PKT, 1, 1, fileName.length() + 1, fileName.c_str())))
        std::cerr << "[-] Error sending file name!" << std::endl;

    sleep(1);

    this->socket.sendFile(fileName, this->socket.getSocketFd());
}

void Client::downloadFile(std::string fileName)
{
    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(REQUEST_FILE, 1, 1, fileName.length() + 1, fileName.c_str())))
        std::cerr << "[-] Error requesting file " << fileName << std::endl;

    this->socket.downloadFile(fileName, this->socket.getSocketFd());
    return;
}

void Client::deleteFile(std::string fileName)
{
    std::string filepath = "sync_dir_" + username + "/" + fileName;

    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(DELETE, 1, 1, filepath.length() + 1, filepath.c_str())))
        std::cerr << "[-] Error sending command!" << std::endl;

    if (FileHandler::deleteFile(filepath))
        std::cerr << "[-] Failed to delete file!" << std::endl;
    else
        std::cout << "[+] File deleted successfully." << std::endl;
}
void Client::listServerFiles()
{
    std::string command = "list_server";

    if (!this->socket.sendMessage(this->socket.getSocketFd(),
                                  Packet(COMMAND_PKT, 1, 1, command.length() + 1, command.c_str())))
        std::cerr << "[-] Error sending command!" << std::endl;

    Packet pkt = this->socket.receiveMessage(this->socket.getSocketFd());
    std::cout << "Server files:" << std::endl;
    std::cout << pkt.payload.get() << std::endl;
}
void Client::listClientFiles()
{
    std::string filepath = "sync_dir_" + username;

    std::vector<std::string> files = FileHandler::getDetailedFileList(filepath);
    std::cout << "[#] Local files:" << std::endl;
    for (std::string str : files)
    {
        std::cout << str << std::endl;
    }
}
void Client::getSyncDir()
{
    std::string syncDir = "sync_dir_" + username;

    if (!std::filesystem::exists(syncDir))
    {
        if (!std::filesystem::create_directory(syncDir))
        {
            std::cerr << "[-] Failed to create sync_dir directory!" << std::endl;
        }
    }

    std::string syncDirPath = std::filesystem::absolute(syncDir).string();

    std::vector<Packet> serverFilesInfo;
    std::cout << "[#] Syncing..." << std::endl;
    while (true)
    {

        Packet receivedPacket = this->socket.receiveMessage(this->socket.getSocketFd());

        if (receivedPacket.type == FILE_INFO_PKT)
            serverFilesInfo.push_back(std::move(receivedPacket));

        if (receivedPacket.type == FINAL_PKT)
            break;
    }

    for (const Packet &pkt : serverFilesInfo)
    {
        // Index 0 = Filepath / Index 1 = File Size / Index 2 = File Modified Time
        std::vector<std::string> fileInfo = FileHandler::parseFileInfo(pkt.payload.get());

        if (std::filesystem::exists(fileInfo[0]))
        {
            std::vector<std::string> localFile = FileHandler::getFileInfo(fileInfo[0]);
            if (fileInfo[1] == localFile[1])
            {
                continue;
            }
        }

        this->socket.getSyncFile(fileInfo[0], this->username);
    }

    std::cout << "[+] Sync complete." << std::endl;
}

void Client::requestSync()
{
    this->socket.sendMessage(this->socket.getSocketFd(), Packet(SYNC_PKT));
}


 std::string Client::getUsername()
 {
    return this->username;
 }