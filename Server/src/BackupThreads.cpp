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
    int timeoutSecond = 10;

    while (true)
    {
        server->serverSocket.sendMessage(server->serverSocket.getSocketFd(), Packet(HEARTBEAT));

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(server->serverSocket.getSocketFd(), &readSet);

        struct timeval timeout;
        timeout.tv_sec = timeoutSecond;
        timeout.tv_usec = 0;

        int selectResult = select(server->serverSocket.getSocketFd() + 1, &readSet, NULL, NULL, &timeout);

        if (selectResult == -1)
        {
            std::cerr << "[-] Error while using select()" << std::endl;
        }
        else if (selectResult == 0)
        {
            std::cout << "[+] Timeout: No heartbeat response received within " << timeoutSecond << " seconds." << std::endl;
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
            }
        }

        sleep(5);
    }

    return nullptr;
}

void *backupDataSync(void *arg)
{
    return nullptr;
}

void *backupMapReceive(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    while (true)
    {
        Packet pkt = server->serverSocket.receiveMessage(server->serverSocket.getSocketFd());

        if (pkt.type == BACKUP_MAP)
        {
            std::cout << "[+] Receive a list of backup sockets" << std::endl;
            server->serverSocket.setBackupSocketMap(server->serverSocket.stringToUnorderedMap(pkt.payload.get()));
        }

        sleep(5);
    }
}
