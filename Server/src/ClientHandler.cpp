#include "../headers/ClientHandler.hpp"

ClientHandler::ClientHandler(int clientSocket, ServerSocket *serverSocket)
{
    this->clientSocket = clientSocket;
    this->serverSocket = serverSocket;
}

void ClientHandler::handleClient()
{

    while (true)
    {
        Packet pkt = serverSocket->receiveMessage(clientSocket);

        if (pkt.type == FAILURE)
            break;
        if (pkt.type == DISCONNECTED)
            break;

        if (pkt.type == COMMAND_PKT)
        {
            std::string payload(pkt.payload.get());

            std::cout << "[" << clientUsername << "] requested " << payload << "." << std::endl;

            if (payload == "upload")
            {
                Packet fileNamePkt = serverSocket->receiveMessage(clientSocket);
                std::string filename = FileHandler::extractFilename(fileNamePkt.payload.get());
                serverSocket->receiveFile(filename, clientSocket, this->clientUsername);
                continue;
            }
            else if (payload == "list_server")
            {
                this->listServer(this->clientUsername);
                continue;
            }
            else if (payload == "exit")
            {
                std::cout << "Disconnecting " << clientUsername << std::endl;
                return;
            }
        }

        if (pkt.type == SYNC_PKT)
        {
            this->getSyncDir();
        }

        if (pkt.type == DELETE)
        {
            std::cout << "[+] " << clientUsername << " requested to delete file " << pkt.payload.get() << std::endl;
            if (FileHandler::deleteFile(pkt.payload.get()))
                std::cerr << "[-] Failed to delete file." << std::endl;
            else
                std::cout << "[+] File deleted successfully." << std::endl;
            continue;
        }
        if (pkt.type == REQUEST_FILE)
        {
            serverSocket->sendUserFile(this->clientUsername, clientSocket, pkt.payload.get());
            continue;
        }

        else
            continue;
    }

    return;
}

int ClientHandler::getClientSocket()
{
    return this->clientSocket;
}

void ClientHandler::setClientUsername(std::string username)
{
    this->clientUsername = username;
}

std::string ClientHandler::getClientUsername()
{
    return this->clientUsername;
}

void ClientHandler::getSyncDir()
{
    std::string syncDir = "sync_dir_" + clientUsername;

    if (!std::filesystem::exists(syncDir))
    {
        if (std::filesystem::create_directory(syncDir))
        {
            std::cout << "[+] Sync dir created for " << clientUsername << " - " << syncDir << std::endl;
        }
        else
        {
            std::cout << "[-] Failed to create sync dir!" << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "[#] " << syncDir << " found." << std::endl;
    }

    std::cout << "[#] Attempting to sync directories..." << std::endl;
    std::vector<std::string> filesInSyncDir = FileHandler::getFileList(syncDir);

    for (std::string str : filesInSyncDir)
    {
        this->serverSocket->sendMessage(this->clientSocket, Packet(FILE_INFO_PKT, 1, 1, str.length() + 1, str.c_str()));
        sleep(1);
    }

    this->serverSocket->sendMessage(this->clientSocket, Packet(FINAL_PKT));
    std::cout << "[+] Sync successful." << std::endl;
    return;
}

void ClientHandler::listServer(std::string username)
{
    std::string filepath = "sync_dir_" + username;

    std::vector<std::string> files = FileHandler::getDetailedFileList(filepath);
    std::string newPayload = std::accumulate(files.begin(), files.end(), std::string(),
                                             [](const std::string &acc, const std::string &str)
                                             { return acc + str + "\n"; });
    this->serverSocket->sendMessage(this->clientSocket, Packet(FILE_LIST, 1, 1, newPayload.length(), newPayload.c_str()));
    return;
}