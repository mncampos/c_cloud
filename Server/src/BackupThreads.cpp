#include <iostream>
#include "../headers/Server.hpp"
#include "../headers/BackupThreads.hpp"

void *backupElectionSocket(void *arg)
{
    return nullptr;
}

void *backupHeartBeat(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    while (true)
    {
        server->serverSocket.sendMessage(server->serverSocket.getSocketFd(), Packet(HEARTBEAT));

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(server->serverSocket.getSocketFd(), &readSet);

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int selectResult = select(server->serverSocket.getSocketFd() + 1, &readSet, NULL, NULL, &timeout);

        if (selectResult == -1)
        {
            std::cerr << "[-] Error while using select()" << std::endl;
        }
        else if (selectResult == 0)
        {
            std::cout << "[+] Timeout: No heartbeat response received within " << 5 << " seconds." << std::endl;
            // @todo: election
            // runElection() here!
        }
        else
        {
            if (FD_ISSET(server->serverSocket.getSocketFd(), &readSet))
            {
                Packet receivedPacket = server->serverSocket.receiveMessage(server->serverSocket.getSocketFd());
                if (receivedPacket.type == HEARTBEAT)
                {
                    std::cout << "[+] Received heartbeat response." << std::endl;
                }
                else
                {
                    std::cerr << "[-] Unexpected packet received." << std::endl;
                }
            }
        }

        sleep(10);
    }

    return nullptr;
}

void *backupDataSync(void *arg)
{
    return nullptr;
}
