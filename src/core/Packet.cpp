#include "core/Packet.h"
#include "core/SimulationEngine.h"

uint32_t Packet::nextSequenceNumber_ = 0;

Packet::Packet(const std::string& srcMAC, const std::string& dstMAC,
               const std::string& srcIP, const std::string& dstIP,
               const std::string& data)
    : srcMAC_(srcMAC), dstMAC_(dstMAC), srcIP_(srcIP), dstIP_(dstIP), data_(data),
      size_(data.size() + 54),  // 14 (MAC) + 20 (IP) + 20 (TCP) = 54 bytes overhead
      creationTime_(SimulationEngine::getInstance().getCurrentTime()),
      sequenceNumber_(nextSequenceNumber_++) {}

std::string Packet::getSrcMAC() const {
    return srcMAC_;
}

std::string Packet::getDstMAC() const {
    return dstMAC_;
}

std::string Packet::getSrcIP() const {
    return srcIP_;
}

std::string Packet::getDstIP() const {
    return dstIP_;
}

std::string Packet::getData() const {
    return data_;
}

uint32_t Packet::getSize() const {
    return size_;
}

double Packet::getCreationTime() const {
    return creationTime_;
}

double Packet::getDeliveryTime() const {
    return deliveryTime_;
}

PacketState Packet::getState() const {
    return state_;
}

int Packet::getTTL() const {
    return ttl_;
}

uint32_t Packet::getSequenceNumber() const {
    return sequenceNumber_;
}

void Packet::setState(PacketState state) {
    state_ = state;
}

void Packet::setDeliveryTime(double time) {
    deliveryTime_ = time;
}

void Packet::decrementTTL() {
    if (ttl_ > 0) ttl_--;
}

std::string Packet::stateToString() const {
    switch (state_) {
        case PacketState::CREATED: return "CREATED";
        case PacketState::QUEUED: return "QUEUED";
        case PacketState::TRANSMITTING: return "TRANSMITTING";
        case PacketState::DELIVERED: return "DELIVERED";
        case PacketState::LOST: return "LOST";
        case PacketState::COLLIDED: return "COLLIDED";
        default: return "UNKNOWN";
    }
}

std::string Packet::toDetailedString() const {
    std::string result = "Packet #" + std::to_string(sequenceNumber_) + " [" + stateToString() + "]\n";
    result += "  Source:      " + srcIP_ + " (" + srcMAC_ + ")\n";
    result += "  Destination: " + dstIP_ + " (" + dstMAC_ + ")\n";
    result += "  Size:        " + std::to_string(size_) + " bytes\n";
    result += "  Created:     " + std::to_string(creationTime_) + " us\n";
    if (deliveryTime_ >= 0) {
        result += "  Delivered:   " + std::to_string(deliveryTime_) + " us\n";
        result += "  Delay:       " + std::to_string(deliveryTime_ - creationTime_) + " us\n";
    }
    result += "  TTL:         " + std::to_string(ttl_) + "\n";
    result += "  Data:        \"" + data_ + "\"";
    return result;
}
