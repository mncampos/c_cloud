#include <iostream>
#include "../headers/Server.hpp"

Server::Server() : serverSocket(PORT), backupSocket(BACKUP_PORT) {}

// Subrotina para lidar com conexão do cliente
void *handleClient(void *arg)
{
    ClientHandler *clientHandler = reinterpret_cast<ClientHandler *>(arg);
    std::cout << "[+] New connection received!" << std::endl;

    Packet receivedPacket = clientHandler->serverSocket->receiveMessage(clientHandler->getClientSocket());
    std::cout << "[+] User " << receivedPacket.payload.get() << " connected!" << std::endl;
    clientHandler->setClientUsername(receivedPacket.payload.get());
    clientHandler->serverSocket->addClientSocket(receivedPacket.payload.get(), clientHandler->getClientSocket());
    clientHandler->getSyncDir();

    clientHandler->handleClient();

    clientHandler->serverSocket->removeClientSocket(clientHandler->getClientSocket());
    close(clientHandler->getClientSocket());
    std::cout << "[+] User " << clientHandler->getClientUsername() << " successfully disconnected." << std::endl;

    pthread_exit(nullptr);
}

// Subrotina para lidar com conexão de nova backup
void *handleBackup(void *arg)
{
    // @todo fazer o handler para a backup
    BackupHandler *backupHandler = reinterpret_cast<BackupHandler *>(arg);
    std::cout << "[+] New backup received!" << std::endl;

    backupHandler->handleBackup();

    pthread_exit(nullptr);
}

void *backupManager(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    // Wait for backup connections
    if (!server->backupSocket.listen(BACKUP_CONNECTIONS_LIMIT))
        std::cout << "[-] Error listening to backup connections!" << std::endl;
    else
        std::cout << "[+] Listening for backup connections on port " << BACKUP_PORT << std::endl;

    int backupSocketFd = 0; // Probably should be protected with a mutex or something

    while (true)
    {
        // Blocked until a new connection is made
        backupSocketFd = server->backupSocket.accept();
        if (backupSocketFd != -1)
            std::cerr << "[+] Error accepting backup connections!" << std::endl;

        BackupHandler *backupHandler = new BackupHandler(backupSocketFd, &server->backupSocket);

        // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente
        pthread_t backupThread;
        if (pthread_create(&backupThread, nullptr, handleBackup, reinterpret_cast<void *>(backupHandler)) != 0)
        {
            std::cerr << "[-] Thread creation fail!" << std::endl;
            delete backupHandler;
            close(backupSocketFd);
        }
        pthread_detach(backupThread);
    }
}

void *clientManager(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    // Wait for client connections
    if (!server->serverSocket.listen(CONNECTIONS_LIMIT))
        std::cout << "[-] Error listening to connections!" << std::endl;
    else
        std::cout << "[+] Server online and listening for connections on port " << PORT << std::endl;

    int clientSocketFd = 0; // Probably should be protected with a mutex or something

    while (true)
    {
        // Blocked until a new connection is made
        clientSocketFd = server->serverSocket.accept();
        if (clientSocketFd == -1)
            std::cerr << "[-] Error accepting connections!" << std::endl;

        ClientHandler *clientHandler = new ClientHandler(clientSocketFd, &server->serverSocket);

        // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente
        pthread_t clientThread;
        if (pthread_create(&clientThread, nullptr, handleClient, reinterpret_cast<void *>(clientHandler)) != 0)
        {
            std::cerr << "[-] Thread creation fail!" << std::endl;
            delete clientHandler;
            close(clientSocketFd);
        }
        pthread_detach(clientThread);
    }
}

void Server::run()
{
    pthread_t clientManagerThread;
    if (pthread_create(&clientManagerThread, nullptr, clientManager, reinterpret_cast<void *>(this)) != 0)
    {
        std::cerr << "[-] Thread creation fail!" << std::endl;
    }

    // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente

    // Se a conexão ocorreu com sucesso, spawna uma nova thread para A BACKUP
    pthread_t backupManagerThread;
    if (pthread_create(&backupManagerThread, nullptr, backupManager, reinterpret_cast<void *>(this)) != 0)
    {
        std::cerr << "[-] Thread creation fail!" << std::endl;
    }

    pthread_join(clientManagerThread, nullptr);
    pthread_join(backupManagerThread, nullptr);
}

void Server::runBackup(std::string mainServerIp)
{

    std::string serverIp = mainServerIp;

    if (!this->serverSocket.connectBackupToServer(mainServerIp, BACKUP_PORT))
    {
        std::cout << "[-] Error connecting to server!" << std::endl;
        exit(-1);
    }
    else
        std::cout << "[+] Connected to server succesfully!" << std::endl;

    
}

void Server::runElection()
{
    //@todo
}
