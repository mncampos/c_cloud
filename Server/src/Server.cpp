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
    clientHandler->getSyncDir();
    
    clientHandler->handleClient();


    close(clientHandler->getClientSocket());
    std::cout << "[+] Connection ended!";

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

        std::cout << "[+] Created new thread [" << clientThread << "] for handling client number " << clientSocketFd << std::endl;
        pthread_detach(clientThread);
    }
}
