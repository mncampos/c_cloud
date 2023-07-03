#include "../Packet.hpp"

// Constructor
Packet::Packet(uint16_t type, uint16_t seqn, uint32_t total_size, uint16_t length, const char *payload)
    : type(type), seqn(seqn), totalSize(total_size), length(length)
{
    _payload = new char[MAX_PAYLOAD];
    std::memset(_payload, 0, MAX_PAYLOAD);
    if (payload != nullptr && length > 0)
    {
        std::memcpy(_payload, payload, length > MAX_PAYLOAD ? MAX_PAYLOAD : length);
    }
}

// Destructor
Packet::~Packet()
{
    delete[] _payload;
}

// Getters
uint16_t Packet::getType() const
{
    return type;
}

uint16_t Packet::getSeqn() const
{
    return seqn;
}

uint32_t Packet::getTotalSize() const
{
    return totalSize;
}

uint16_t Packet::getLength() const
{
    return length;
}
