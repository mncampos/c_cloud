#include "../headers/Client.hpp"

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "[-] Invalid number of parameters." << std::endl
                  << "[#] Usage: ./myClient <username> <server_ip_address> <port>" << std::endl;
        return -1;
    }

    std::string username = argv[1];
    std::string serverIP = argv[2];
    int port = std::stoi(argv[3]);

    Client client(username, serverIP, port);

    if (!client.socket.connectToServer()){
        std::cout << "[-] Error connecting to server!" << std::endl;
        exit(-1);
    }
    else
        std::cout << "[+] Connected to server succesfully!" << std::endl;

    // Send user info to server
    if (!client.socket.sendUsername(username))
    {
        std::cerr << "[-] Error sending user info!" << std::endl;
        ::close(client.socket.getSocketFd());
        exit(1);
    }

    client.getSyncDir();

    while (true)
    {
        std::string command;
        std::cout << "[+] Enter a command: \n > ";
        std::getline(std::cin, command);

        std::istringstream iss(command);
        std::string cmdName, fileName;

        iss >> cmdName >> fileName;

        std::transform(cmdName.begin(), cmdName.end(), cmdName.begin(), ::tolower);

        if (cmdName == "upload")
            client.uploadFile(fileName);
        else if (cmdName == "download")
            client.downloadFile(fileName);
        else if (cmdName == "delete")
            client.deleteFile(fileName);
        else if (cmdName == "list_server")
            client.listServerFiles();
        else if (cmdName == "list_client")
            client.listClientFiles();
        else if (cmdName == "get_sync_dir")
            client.getSyncDir();
        else if (cmdName == "exit")
            break;
        else
            std::cout << "[-] Invalid command. Try again." << std::endl;
    }

    // Fecha o socket
    std::cout << "[+] Exiting program, goodbye!" << std::endl;
    client.socket.close();

    return 0;
}