#pragma once
#include <string>
#include <vector>

struct RouteEntry {
    std::string destination;
    std::string netmask;
    std::string nextHop;
};

class IPRouting {
public:
    static std::string resolveNextHop(const std::string& destinationIP,
                                       const std::vector<RouteEntry>& table);
    static bool matchNetwork(const std::string& ip, const std::string& network,
                             const std::string& netmask);
};