#include "../Socket.hpp"

Socket::Socket() : socketFd(-1) {}

Socket::~Socket()
{
    close();
}

bool Socket::create()
{
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    return socketFd != -1;
}

bool Socket::close()
{
    if (socketFd != -1)
    {
        if (::close(socketFd) == 0)
        {
            socketFd = -1;
            return true;
        }
    }
    return false;
}

int Socket::getSocketFd()
{
    return this->socketFd;
}
bool Socket::sendFile(std::string filename, int clientSocketFd)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file)
    {
        std::cout << "Error creating file!" << std::endl;
        return false;
    }
    std::streampos fileSize = file.tellg(); // Gets the file size
    uint32_t numPackets = std::ceil(static_cast<double>(fileSize) / MAX_PAYLOAD);
    std::cout << "Sending file : " << filename << " \nSize : " << fileSize << " \nPackets required : " << numPackets << std::endl;
    file.seekg(0, std::ios::beg);

    std::vector<Packet> packets;
    uint16_t sequence = 1;

    for (uint32_t i = 0; i < numPackets; ++i)
    {
        uint16_t length = static_cast<uint16_t>(std::min(static_cast<std::streampos>(MAX_PAYLOAD), fileSize));
        fileSize -= length;
        std::unique_ptr<char[]> payload(new char[length]);
        file.read(payload.get(), length);

        Packet packet(FILE_PKT, sequence, numPackets, length, payload.get());
        packets.push_back(std::move(packet));

        ++sequence;
    }

    for (Packet &p : packets)
    {
        std::cout << "Sending packet number " << p.seqn << " of " << p.totalSize << std::endl;
        std::vector<uint8_t> serializedPacket = p.serialize();

        ssize_t sentBytes = send(clientSocketFd, serializedPacket.data(), serializedPacket.size(), 0);
        if (sentBytes == -1 || sentBytes == 0)
        {
            std::cerr << "Error sending file!" << std::endl;
            return false;
        }
        std::cout << "Sent " << sentBytes << " bytes!" << std::endl;
    }

    file.close();
    return true;
}

bool Socket::receiveFile(std::string filename, int socketFd, std::string username)
{
    std::vector<Packet> filePackets;
    std::cout << "Receiving file : " << filename << std::endl;

    while (true)
    {
        std::vector<uint8_t> dataBuffer(MAX_PAYLOAD + 10);
        ssize_t bytesRead = recv(socketFd, dataBuffer.data(), dataBuffer.size(), 0);

        if (bytesRead == 1)
        {
            std::cerr << "[-] Failed to receive data!" << std::endl;
            return false;
        }

        std::vector<uint8_t> byteStream(dataBuffer.begin(), dataBuffer.begin() + bytesRead);
        Packet assembledPacket = Packet::deserialize(byteStream);

        // std::cout << "[+] Received packet " << assembledPacket.seqn << " / " << assembledPacket.totalSize << " of file " << filename << "." << std::endl;
        filePackets.push_back(std::move(assembledPacket));

        if (assembledPacket.seqn == assembledPacket.totalSize)
            break;
    }

    std::string filePath = "sync_dir_" + username + "/" + filename;
    std::ofstream newFile(filePath, std::ios::binary);
    if (!newFile)
    {
        std::cerr << "[-] Failed creating file" << std::endl;
        return false;
    }

    std::sort(filePackets.begin(), filePackets.end(), [](const Packet &a, const Packet &b)
              { return a.seqn < b.seqn; });

    std::cout << "[+] " << filename << " received succesfully." << std::endl;

    for (const Packet &packet : filePackets)
    {
        newFile.write(packet.payload.get(), packet.length);
    }

    newFile.close();
    return true;
}

Packet Socket::receiveMessage(int clientSocketFd)
{
    std::vector<uint8_t> buf(MAX_PAYLOAD + 10);
    ssize_t bytesRead = recv(clientSocketFd, buf.data(), buf.size(), 0);
    if (bytesRead == -1)
    {
        std::cerr << "[-] Failed to receive message!" << std::endl;
        return Packet(FAILURE);
    }

    if (bytesRead == 0)
    {
        std::cerr << "[-] Client suddenly disconnected!" << std::endl;
        return Packet(FAILURE);
    }

    std::vector<uint8_t> receivedData(buf.begin(), buf.begin() + bytesRead);
    Packet receivedPkt = Packet::deserialize(receivedData);

    if (receivedPkt.type == FILENAME_PKT)
        std::cout << receivedPkt.payload.get() << std::endl;

    return receivedPkt;
}

bool Socket::sendMessage(int socketFd, Packet packet)
{
    std::vector<uint8_t> serializedPacket = packet.serialize();
    return ::send(socketFd, serializedPacket.data(), serializedPacket.size(), 0) > 0;
}