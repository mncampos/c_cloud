#include "../headers/Server.hpp"

Server::Server() : serverSocket(PORT) {}

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

void Server::run()
{
    // Wait for client connections
    if (!serverSocket.listen(CONNECTIONS_LIMIT))
        std::cout << "[-] Error listening to connections!" << std::endl;
    else
        std::cout << "[+] Server online and listening for connections on port " << PORT << std::endl;

    int clientSocketFd = 0; // Probably should be protected with a mutex or something

    while (true)
    {
        // Blocked until a new connection is made
        clientSocketFd = serverSocket.accept();
        if (clientSocketFd == -1)
            std::cout << "[+] Error accepting connections!" << std::endl;
            
        ClientHandler *clientHandler = new ClientHandler(clientSocketFd, &this->serverSocket);

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

void Server::runBackup(std::string mainServerIp)
{

    std::string serverIp = mainServerIp;
    int port = 4000;

    if (!this->serverSocket.connectBackupToServer(mainServerIp, port))
    {
        std::cout << "[-] Error connecting to server!" << std::endl;
        exit(-1);
    }
    else
        std::cout << "[+] Connected to server succesfully!" << std::endl;


}
