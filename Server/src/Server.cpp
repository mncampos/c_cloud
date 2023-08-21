#include <iostream>
#include "../headers/Server.hpp"

Server::Server() : serverSocket(PORT), replicaSocket(REPLICA_PORT) {}

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

// Subrotina para lidar com conexão de nova replica
void *handleReplica(void *arg)
{
    // @todo fazer o handler para a replica
    BackupHandler *backupHandler = reinterpret_cast<BackupHandler *>(arg);
    std::cout << "[+] New backup received!" << std::endl;

    backupHandler->handleBackup();

    pthread_exit(nullptr);
}

void *replicaManager(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    // Wait for replica connections
    if (!server->replicaSocket.listen(REPLICA_CONNECTIONS_LIMIT))
        std::cout << "[-] Error listening to replica connections!" << std::endl;
    else
        std::cout << "[+] Listening for replica connections on port " << REPLICA_PORT << std::endl;

    int replicaSocketFd = 0; // Probably should be protected with a mutex or something

    while (true)
    {
        // Blocked until a new connection is made
        replicaSocketFd = server->replicaSocket.accept();
        if (replicaSocketFd != -1)
            std::cerr << "[+] Error accepting replica connections!" << std::endl;

        BackupHandler *backupHandler = new BackupHandler(replicaSocketFd, &server->replicaSocket);

        // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente
        pthread_t replicaThread;
        if (pthread_create(&replicaThread, nullptr, handleReplica, reinterpret_cast<void *>(backupHandler)) != 0)
        {
            std::cerr << "[-] Thread creation fail!" << std::endl;
            delete backupHandler;
            close(replicaSocketFd);
        }
        pthread_detach(replicaThread);
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

    // Se a conexão ocorreu com sucesso, spawna uma nova thread para A REPLICA
    pthread_t replicaManagerThread;
    if (pthread_create(&replicaManagerThread, nullptr, replicaManager, reinterpret_cast<void *>(this)) != 0)
    {
        std::cerr << "[-] Thread creation fail!" << std::endl;
    }

    pthread_join(clientManagerThread, nullptr);
    pthread_join(replicaManagerThread, nullptr);
}

void Server::runBackup(std::string mainServerIp)
{

    std::string serverIp = mainServerIp;

    if (!this->serverSocket.connectBackupToServer(mainServerIp, REPLICA_PORT))
    {
        std::cout << "[-] Error connecting to server!" << std::endl;
        exit(-1);
    }
    else
        std::cout << "[+] Connected to server succesfully!" << std::endl;
}
