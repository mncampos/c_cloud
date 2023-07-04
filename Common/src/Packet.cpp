#include "../Packet.hpp"

// Constructor
Packet::Packet(uint16_t type, uint16_t seqn, uint32_t total_size, uint16_t length, const char* payload)
    : type(type), seqn(seqn), totalSize(total_size), length(length)
{
    this->payload = std::make_unique<char[]>(length + 1);  // +1 for null terminator
    std::memcpy(this->payload.get(), payload, length);
    this->payload[length] = '\0';  // Null-terminate the payload
}

Packet::Packet()
    : type(0), seqn(0), totalSize(0), length(0), payload(nullptr)
{
}


std::vector<uint8_t> Packet::serialize()
{
   std::vector<uint8_t> serializedPacket;
    serializedPacket.reserve(sizeof(type) + sizeof(seqn) + sizeof(totalSize) + sizeof(length) + length);

    uint16_t s_type = htons(type);
    uint16_t s_seqn = htons(seqn);
    uint32_t s_totalSize = htons(totalSize);
    uint16_t s_length = htons(length);

    serializedPacket.insert(serializedPacket.end(), reinterpret_cast<const uint8_t*>(&s_type), reinterpret_cast<const uint8_t*>(&s_type) + sizeof(s_type));
    serializedPacket.insert(serializedPacket.end(), reinterpret_cast<const uint8_t*>(&s_seqn), reinterpret_cast<const uint8_t*>(&s_seqn) + sizeof(s_seqn));
    serializedPacket.insert(serializedPacket.end(), reinterpret_cast<const uint8_t*>(&s_totalSize), reinterpret_cast<const uint8_t*>(&s_totalSize) + sizeof(s_totalSize));
    serializedPacket.insert(serializedPacket.end(), reinterpret_cast<const uint8_t*>(&s_length), reinterpret_cast<const uint8_t*>(&s_length) + sizeof(s_length));
    serializedPacket.insert(serializedPacket.end(), payload.get(), payload.get() + length);

    return serializedPacket;
}
