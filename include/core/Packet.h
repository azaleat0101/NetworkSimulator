#pragma once
#include <string>
#include <vector>
#include <cstdint>

enum class PacketState {
    CREATED,
    QUEUED,
    TRANSMITTING,
    DELIVERED,
    LOST,
    COLLIDED
};

class Packet {
public:
    Packet(const std::string& srcMAC, const std::string& dstMAC,
           const std::string& srcIP, const std::string& dstIP,
           const std::string& data);

    // Getters
    std::string getSrcMAC() const;
    std::string getDstMAC() const;
    std::string getSrcIP() const;
    std::string getDstIP() const;
    std::string getData() const;
    uint32_t getSize() const;
    double getCreationTime() const;
    double getDeliveryTime() const;
    PacketState getState() const;
    int getTTL() const;
    uint32_t getSequenceNumber() const;

    // Setters
    void setState(PacketState state);
    void setDeliveryTime(double time);
    void decrementTTL();

    // Utilities
    std::string stateToString() const;
    std::string toDetailedString() const;

private:
    std::string srcMAC_;
    std::string dstMAC_;
    std::string srcIP_;
    std::string dstIP_;
    std::string data_;
    uint32_t size_;
    double creationTime_;
    double deliveryTime_ = -1.0;
    PacketState state_ = PacketState::CREATED;
    int ttl_ = 64;
    uint32_t sequenceNumber_;

    static uint32_t nextSequenceNumber_;
};
