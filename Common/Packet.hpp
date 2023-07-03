#ifndef PACKET_HPP
#define PACKET_HPP

#include <cstdint>
#include <cstring>

const uint16_t MAX_PAYLOAD = 1024; // 1kb

enum PacketType
{
    USERNAME_PKT = 1, // Packet that sends a username
    FILE_PKT,         // Sends a file
    COMMAND_PKT,      // Sends a command
    NOTIFY_PT,        // Sends a notification, usually if the user disconnected
};

class Packet
{
public:
    uint16_t type;      // Packet type (see enums)
    uint16_t seqn;      // Sequence number
    uint32_t totalSize; // Total number of fragments
    uint16_t length;    // Payload size
    char *_payload;     // Data

    Packet(uint16_t type, uint16_t seqn, uint32_t total_size, uint16_t length, const char *payload);

    ~Packet();

    // Getters
    uint16_t getType() const;
    uint16_t getSeqn() const;
    uint32_t getTotalSize() const;
    uint16_t getLength() const;
};

#endif
