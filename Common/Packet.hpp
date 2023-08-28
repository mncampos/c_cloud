#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <iostream>
#include <memory>

const uint16_t MAX_PAYLOAD = 1024; // 1kb

enum PacketType
{
    USERNAME_PKT = 1, // Packet that sends a username
    FILE_PKT,         // Sends a file
    COMMAND_PKT,      // Sends a command
    NOTIFY_PKT,       // Sends a notification, like an update
    FAILURE,          // Packet failed
    FILENAME_PKT,     // A Packet that contains a filename, usually sent before a file
    DISCONNECTED,     // A Packet that nofities about the client DC
    FILE_INFO_PKT,    // A Packet that contains file information such as size and modified time
    FINAL_PKT,        // Notifies that a certain transaction has finished
    REQUEST_FILE,     // Requests a file from the user dir, contains the filename
    FILE_LIST,        // Contains a payload of filenames
    DELETE,           // Contains a request to delete a file
    SYNC_PKT,         // Requests a sync call from the server
    ELECTION,         // Election message
    HEARTBEAT,        // Heartbeat message
    IP_PKT,           // Packet that sends a IP
    SYNC_BKP_PKT,     // request a sync call for the backups
};

class Packet
{
public:
    uint16_t type;                   // Packet type (see enums)
    uint16_t seqn;                   // Sequence number
    uint32_t totalSize;              // Total number of fragments
    uint16_t length;                 // Payload size
    std::unique_ptr<char[]> payload; // Data

    Packet(uint16_t type, uint16_t seqn, uint32_t total_size, uint16_t length, const char *payload);
    Packet();
    Packet(uint16_t type);
    std::vector<uint8_t> serialize(); // Serialize the packet into network data format

    static Packet deserialize(const std::vector<uint8_t> &serializedPacket)
    {
        Packet packet;

        packet.type = ntohs(*reinterpret_cast<const uint16_t *>(serializedPacket.data()));
        packet.seqn = ntohs(*reinterpret_cast<const uint16_t *>(serializedPacket.data() + sizeof(packet.type)));
        packet.totalSize = ntohl(*reinterpret_cast<const uint32_t *>(serializedPacket.data() + sizeof(packet.type) + sizeof(packet.seqn)));
        packet.length = ntohs(*reinterpret_cast<const uint16_t *>(serializedPacket.data() + sizeof(packet.type) + sizeof(packet.seqn) + sizeof(packet.totalSize)));

        packet.payload = std::make_unique<char[]>(packet.length);
        std::memcpy(packet.payload.get(), serializedPacket.data() + sizeof(packet.type) + sizeof(packet.seqn) + sizeof(packet.totalSize) + sizeof(packet.length), packet.length);

        return packet;
    }

    friend std::ostream &operator<<(std::ostream &os, const Packet &packet)
    {
        os << "Packet Info:" << std::endl;
        os << "Type: " << packet.type << std::endl;
        os << "Sequence Number: " << packet.seqn << std::endl;
        os << "Total Size: " << packet.totalSize << std::endl;
        os << "Payload Length: " << packet.length << std::endl;

        std::string payloadStr(packet.payload.get(), packet.length);
        os << "Payload: " << payloadStr << std::endl;

        return os;
    }
};

#endif
