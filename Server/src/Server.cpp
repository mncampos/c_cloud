#include "../headers/Server.hpp"

Server::Server() : serverSocket(PORT) {}

// Subrotina para lidar com conexão do cliente
void *handleClient(void *arg)
{
    ClientHandler *clientHandler = reinterpret_cast<ClientHandler *>(arg);
    std::cout << "New connection received!" << std::endl;

    while (true)
    {
        clientHandler->handleClient();
        sleep(5);
    }

    close(clientHandler->getClientSocket());
    std::cout << "Connection ended!";

    pthread_exit(nullptr);
}

void Server::run()
{
    // Aguarda por conexões do cliente
    if (!serverSocket.listen(CONNECTIONS_LIMIT))
        std::cout << "Error listening to connections!" << std::endl;
    else
        std::cout << "Server online and listening for connections on port " << PORT << std::endl;

    while (true)
    {
        // Aguarda por conexões (bloqueia até vir uma)
        if (!serverSocket.accept())
            std::cout << "Error accepting connections!" << std::endl;

        ClientHandler *clientHandler = new ClientHandler(serverSocket.getClientSocketFd());

        // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente
        pthread_t clientThread;
        if (pthread_create(&clientThread, nullptr, handleClient, reinterpret_cast<void *>(clientHandler)) != 0)
        {
            std::cerr << "Thread creation fail!" << std::endl;
            delete clientHandler;
            close(serverSocket.getClientSocketFd());
        }

        std::cout << "Created new thread [" << clientThread << "] for handling client number " << serverSocket.getClientSocketFd() << std::endl;
        pthread_detach(clientThread);
    }
}
