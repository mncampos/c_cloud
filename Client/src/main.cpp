#include "../headers/Client.hpp"

pthread_mutex_t syncMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t syncCond = PTHREAD_COND_INITIALIZER;
bool isSyncReady = false;

void *monitorSyncDir(void *arg)
{
    Client *client = static_cast<Client *>(arg);

    pthread_mutex_lock(&syncMutex);

    while (!isSyncReady)
        pthread_cond_wait(&syncCond, &syncMutex);
    pthread_mutex_unlock(&syncMutex);

    while (true)
    {
        std::string event = FileHandler::monitorDirectory("sync_dir_" + client->getUsername());

        client->socket.sendMessage(client->socket.getSocketFd(), Packet(NOTIFY_PKT, 1, 1, event.length() + 1, event.c_str()));
        sleep(1);

        std::istringstream iss(event);
        std::string eventName, fileName;

        if (std::getline(iss, eventName, ':') && std::getline(iss, fileName))
        {
            if (eventName == "insert" || eventName == "update")
            {
                client->socket.sendUserFile(client->getUsername(), client->socket.getSocketFd(), fileName);
            }
        }
    }
    pthread_exit(nullptr);
}

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


    if (!client.socket.connectToServer())
    {
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

    // Create monitoring thread
    pthread_t monitorThread;
    if (pthread_create(&monitorThread, nullptr, monitorSyncDir, static_cast<void *>(&client)) != 0)
    {
        std::cerr << "Failed to create monitor thread!" << std::endl;
        exit(1);
    }

    pthread_detach(monitorThread);

    pthread_mutex_lock(&syncMutex);
    client.getSyncDir();
    isSyncReady = true;
    pthread_cond_signal(&syncCond);
    pthread_mutex_unlock(&syncMutex);

    while (true)
    {
        std::string command;
        std::cout << "[+] Enter a command:" << std::endl;
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
        {
            client.requestSync();
            client.getSyncDir();
        }
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