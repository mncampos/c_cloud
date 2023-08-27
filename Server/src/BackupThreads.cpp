#include <iostream>
#include "../headers/Server.hpp"
#include "../headers/BackupThreads.hpp"
#include "../headers/RingHandler.hpp"

void *backupElectionSocket(void *arg)
{
    Server *server = reinterpret_cast<Server *>(arg);

    // Wait for client connections
    if (!server->ringSocket.listen(CONNECTIONS_LIMIT))
        std::cout << "[-] Error listening to connections!" << std::endl;
    else
        std::cout << "[+] Listening to ring conections on port " << RING_PORT << std::endl;

    int ringSocketFd = 0;

    ringSocketFd = server->ringSocket.accept();

    if (ringSocketFd == -1)
        std::cerr << "[-] Error accepting connections!" << std::endl;

    RingHandler *ringHandler = new RingHandler(ringSocketFd, &server->ringSocket);

    Packet receivedPacket = server->ringSocket.receiveMessage(ringSocketFd);
    
    ringHandler->electionIp = receivedPacket.payload.get();

    return static_cast<void*>(ringHandler);

}

void *backupHeartBeat(void *arg)
{
    std::string name;
    return nullptr;
}

void *backupDataSync(void *arg)
{
    std::string name;
    return nullptr;
}