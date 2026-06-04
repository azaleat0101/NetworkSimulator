#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>

class Node;
class Packet;
class TransmissionStrategy;

enum class LinkType {
    SHARED_BUS,
    POINT_TO_POINT
};

class Link {
public:
    Link(const std::string& id, double bandwidthMbps, double lengthMeters, LinkType type);
    ~Link();

    // Node management
    void connectNode(Node* node);
    std::vector<Node*>& getNodes();

    // Packet transmission (new)
    void transmitPacket(std::shared_ptr<Packet> packet);
    std::shared_ptr<Packet> receivePacket();

    // Legacy transmission (kept for compatibility)
    void transmit(const std::string& srcId, const std::string& dstId, const std::string& data);

    // Link state
    bool isBusy() const;
    void setBusy(bool busy);
    size_t getQueueSize() const;

    // Properties
    std::string getId() const;
    double getBandwidth() const;
    double getPropagationDelay() const;
    double getTransmissionDelay(uint32_t packetSize) const;
    LinkType getType() const;

    // Strategy pattern for transmission protocols
    void setStrategy(std::unique_ptr<TransmissionStrategy> strategy);
    TransmissionStrategy* getStrategy() const;

    // Statistics
    int getTotalTransmitted() const;
    int getTotalCollisions() const;
    double getUtilization() const;

private:
    std::string id_;
    double bandwidthMbps_;              // Bandwidth in Mbps
    double lengthMeters_;               // Cable length in meters
    LinkType type_;
    std::vector<Node*> nodes_;
    bool busy_ = false;
    std::queue<std::shared_ptr<Packet>> packetQueue_;
    std::unique_ptr<TransmissionStrategy> strategy_;

    // Statistics
    int totalTransmitted_ = 0;
    int totalCollisions_ = 0;
    double totalTransmissionTime_ = 0.0;
    double creationTime_;

    static constexpr double SPEED_OF_LIGHT = 200000.0;  // km/s (in optical fiber)
};
