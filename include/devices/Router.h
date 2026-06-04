#pragma once
#include "devices/Node.h"
#include <vector>
#include <string>
#include <memory>

class Packet;

struct RoutingEntry {
    std::string destinationNetwork;
    std::string netmask;
    std::string nextHop;
    std::string interfaceName;
};

class Router : public Node {
public:
    Router(const std::string& id, const std::string& mac, const std::string& ip);

    void sendPacket(const std::string& dstIP, const std::string& data) override;
    void receivePacket(const std::string& srcIP, const std::string& data) override;

    // Real packet routing
    void routeRealPacket(std::shared_ptr<Packet> packet);

    void addRoute(const RoutingEntry& entry);
    void printRoutingTable() const;

private:
    std::vector<RoutingEntry> routingTable_;
    std::string findNextHop(const std::string& dstIP) const;
};
