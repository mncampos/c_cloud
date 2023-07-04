#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include <arpa/inet.h>
#include <memory>

const uint16_t MAX_PAYLOAD = 1024; // 1kb

enum PacketType
{
    USERNAME_PKT = 1, // Packet that sends a username
    FILE_PKT,         // Sends a file
    COMMAND_PKT,      // Sends a command
    NOTIFY_PKT,        // Sends a notification, usually if the user disconnected
    FAILURE,          // Packet failed
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

        packet.payload = std::make_unique<char[]>(packet.length + 1); // +1 for null terminator
        std::memcpy(packet.payload.get(), serializedPacket.data() + sizeof(packet.type) + sizeof(packet.seqn) + sizeof(packet.totalSize) + sizeof(packet.length), packet.length);
        packet.payload[packet.length] = '\0'; // Null-terminate the payload

        return packet;
    }
};

#endif
