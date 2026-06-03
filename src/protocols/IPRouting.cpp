#include "protocols/IPRouting.h"
#include "utils/Logger.h"
#include <sstream>
#include <vector>
#include <cstdint>

// Разделение строки по точке
static std::vector<int> splitIP(const std::string& ip) {
    std::vector<int> result;
    std::istringstream iss(ip);
    std::string token;
    while (std::getline(iss, token, '.')) {
        result.push_back(std::stoi(token));
    }
    return result;
}

// Преобразование IP в uint32_t
static uint32_t ipToUint(const std::string& ip) {
    auto parts = splitIP(ip);
    if (parts.size() != 4) return 0;
    return (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3];
}

bool IPRouting::matchNetwork(const std::string& ip, const std::string& network,
                              const std::string& netmask) {
    uint32_t ipVal = ipToUint(ip);
    uint32_t netVal = ipToUint(network);
    uint32_t maskVal = ipToUint(netmask);
    return (ipVal & maskVal) == (netVal & maskVal);
}

std::string IPRouting::resolveNextHop(const std::string& destinationIP,
                                       const std::vector<RouteEntry>& table) {
    Logger::getInstance().log("  IPRouting: Поиск маршрута для " + destinationIP);

    for (auto& entry : table) {
        if (matchNetwork(destinationIP, entry.destination, entry.netmask)) {
            Logger::getInstance().log("  IPRouting: Найден маршрут -> " + entry.nextHop);
            return entry.nextHop;
        }
    }

    Logger::getInstance().log("  IPRouting: Маршрут не найден, используется default gateway");
    return "0.0.0.0";
}