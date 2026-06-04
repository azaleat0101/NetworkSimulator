#include "devices/Router.h"
#include "core/Packet.h"
#include "network/Link.h"
#include "network/Network.h"
#include "protocols/IPRouting.h"
#include "utils/Logger.h"
#include "utils/StatisticsCollector.h"
#include <iostream>
#include <iomanip>

Router::Router(const std::string& id, const std::string& mac, const std::string& ip)
    : Node(id, mac, ip) {}

void Router::sendPacket(const std::string& dstIP, const std::string& data) {
    std::string nextHop = findNextHop(dstIP);
    Logger::getInstance().log("Router " + id_ + " [" + ipAddress_ +
                              "] маршрутизирует -> " + dstIP + " через " + nextHop);
}

void Router::receivePacket(const std::string& srcIP, const std::string& data) {
    Logger::getInstance().log("Router " + id_ + " [" + ipAddress_ +
                              "] получил пакет от " + srcIP);
}

void Router::routeRealPacket(std::shared_ptr<Packet> packet) {
    if (!packet || !link_) {
        Logger::getInstance().log("ERROR: Router не может маршрутизировать пакет!");
        return;
    }

    std::string dstIP = packet->getDstIP();
    std::string nextHop = findNextHop(dstIP);
    
    double now = SimulationEngine::getInstance().getCurrentTime();
    Logger::getInstance().log("[" + std::to_string(now) + " us] Router " + id_ +
                              " маршрутизирует пакет #" + 
                              std::to_string(packet->getSequenceNumber()) +
                              " для " + dstIP + " через next hop " + nextHop);

    // Пакет уже передан по каналу, маршрутизатор просто логирует событие
    StatisticsCollector::getInstance().recordDelivered(packet->getSrcIP(), dstIP);
}

void Router::addRoute(const RoutingEntry& entry) {
    routingTable_.push_back(entry);
    Logger::getInstance().log("Router " + id_ + ": добавлен маршрут к " + 
                              entry.destinationNetwork);
}

void Router::printRoutingTable() const {
    std::cout << "\n╔══════════════ Таблица маршрутизации: " << id_ << " ══════════════╗\n";
    std::cout << "║ " << std::setw(18) << "Сеть" << " " << std::setw(18) << "Маска"
              << " " << std::setw(18) << "Next Hop" << " " << std::setw(12) << "Интерфейс ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════════════╣\n";
    for (const auto& entry : routingTable_) {
        std::cout << "║ " << std::setw(18) << entry.destinationNetwork
                  << " " << std::setw(18) << entry.netmask
                  << " " << std::setw(18) << entry.nextHop
                  << " " << std::setw(12) << entry.interfaceName << " ║\n";
    }
    std::cout << "╚═════════════���════════════════════════════════════════════════════╝\n";
}

std::string Router::findNextHop(const std::string& dstIP) const {
    // Преобразуем routingTable_ в формат для IPRouting
    std::vector<RouteEntry> entries;
    for (const auto& r : routingTable_) {
        entries.push_back({r.destinationNetwork, r.netmask, r.nextHop});
    }
    return IPRouting::resolveNextHop(dstIP, entries);
}
