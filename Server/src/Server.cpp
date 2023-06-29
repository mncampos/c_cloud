#include "../headers/Server.hpp"

Server::Server()
{
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
}

bool Server::start()
{
    // Criação do Socket
    if (serverSocket == -1)
    {
        std::cerr << "Error creating server socket!" << std::endl;
        return false;
    }

    // Endereço do servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Binda o socket ao endereço
    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error binding socket to the specified address!" << std::endl;
        close(serverSocket);
        return false;
    }

    std::cout << "Server successfully online. Listening for connections..." << std::endl;
    return true;
}

// Subrotina para lidar com conexão do cliente
void *handleClient(void *arg)
{
    ClientHandler *clientHandler = reinterpret_cast<ClientHandler *>(arg);
    std::cout << "New connection received!";

    while (true)
    {
        clientHandler->handleClient();
        sleep(1);
    }
    
    close(clientHandler->getClientSocket());
    std::cout << "Connection ended!";

    pthread_exit(nullptr);
}

void Server::run()
{

    while (true)
    {
        // Aguarda por conexões do cliente
        listen(serverSocket, CONNECTIONS_LIMIT);

        sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddress), &clientAddressLength);
        if (clientSocket == -1)
        {
            std::cerr << "Error accepting connection!"
                      << " Code : " << clientSocket << std::endl;
            close(serverSocket);
            return;
        }

        ClientHandler *clientHandler = new ClientHandler(clientSocket);

        // Se a conexão ocorreu com sucesso, spawna uma nova thread para o cliente
        pthread_t clientThread;
        if (pthread_create(&clientThread, nullptr, handleClient, reinterpret_cast<void *>(clientHandler)) != 0)
        {
            std::cerr << "Thread creation fail!" << std::endl;
            delete clientHandler;
            close(clientSocket);
        }

        pthread_detach(clientThread);
    }
}
