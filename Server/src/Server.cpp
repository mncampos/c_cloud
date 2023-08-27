#include <iostream>
#include "../headers/Server.hpp"
#include "../headers/BackupThreads.hpp"

Server::Server() : serverSocket(PORT), backupSocket(BACKUP_PORT), ringSocket(RING_PORT) {}

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
    // @TODO fazer o handler para a backup
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
        if (backupSocketFd == -1)
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
    void *returnIp;
    void *returnHeartbeat;

    std::string serverIp = mainServerIp;

    if (!this->serverSocket.connectBackupToServer(mainServerIp, BACKUP_PORT))
    {
        std::cout << "[-] Error connecting to server!" << std::endl;
        exit(-1);
    }
    else
        std::cout << "[+] Connected to server succesfully!" << std::endl;

    // while(true)

    pthread_t backupElectionSocket_thread;

    // @TODO: base - cria conexao
    //  backupElectionSocket
    //  cria Thread escutando conecção de backups
    //      pthread_join(thread, ip)
    //      if(result == ip) -> runElection(ip)

    if (pthread_create(&backupElectionSocket_thread, nullptr, backupElectionSocket, reinterpret_cast<void *>(this)) != 0)
    {
        std::cerr << "[-] Thread creation fail!" << std::endl;
    }

    pthread_t backupHeartBeat_thread;

    // @TODO: heartbeat
    //  backupHeartBeat
    //  cria Thread de heartbeat
    //      pthread_join(thread, result)
    //      if(result == deuMerda) -> runElection(null)

    if (pthread_create(&backupHeartBeat_thread, nullptr, backupHeartBeat, reinterpret_cast<void *>(this)) != 0)
    {
        std::cerr << "[-] Thread creation fail!" << std::endl;
    }

    pthread_t backupDataSync_thread;

    // @TODO: sincronização
    //  backupDataSync
    //  cria Thread escutando mensagens do servidor principal
    //  lida com os dados recebidos do servidor principal

    pthread_join(backupElectionSocket_thread, &returnIp);
    pthread_join(backupHeartBeat_thread, &returnHeartbeat);
    pthread_join(backupDataSync_thread, nullptr);

    // std::string electionIp = static_cast<std::string>(returnIp);
    // bool heartBeat = static_cast<std::string>(returnIp);

    while (true)
    {
    }
}

void Server::runElection(std::string msgIp)
{
    //@TODO: eleicao

    // INICIO DE EXECUÇAO
    // if(msgIp != null) ->
    //      Envia melhor Ip para o proximo Backup em lista backupSockets
    // if(msgIp == null) ->
    //      Envia o proprio Ip para o proximo Backup em lista backupSockets

    // RECEBIMENTO DE MENSAGENS
    // ELECTION:
    //      if(msgIp > this.ip) ->
    //          Repassa Ip da mensagem
    //      if(msgIp < this.ip) ->
    //          Envia o proprio Ip
    //      if(msgIp == this.ip) ->
    //          Envia mensagem ELECTED(this.ip), passa a ser servidor principal (rodar Server.run)
    //          Notifica front end socket 6666 que existe novo principal
    // ELECTED(electedIp):
    //      if(proximoNoAnel.ip == electedIp) ->
    //          Volta a executar backup (Server.runBackup(electedIp))
    //      if(proximoNoAnel.ip != electedIp) ->
    //          Envia mensagem ELECTED(electedIp) para o proximo backup
    //          Volta a executar backup (Server.runBackup(electedIp))
}
