#include "../headers/Client.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Invalid number of parameters." << std::endl
                  << "Usage: ./myClient <username> <server_ip_address> <port>" << std::endl;
        return -1;
    }

    std::string username = argv[1];
    std::string serverIP = argv[2];
    int port = std::stoi(argv[3]);

    Client client(username, serverIP, port);

    if (!client.socket.connectToServer())
        std::cout << "Error connecting to server!" << std::endl;
    else
        std::cout << "Connected to server succesfully!" << std::endl;

    // Send user info to server
    if (!client.socket.sendUsername(username))
    {
        std::cerr << "Error sending user info!" << std::endl;
        ::close(client.socket.getSocketFd());
        exit(1);
    }

    sleep(30);

    // Fecha o socket
    client.socket.close();

    return 0;
}