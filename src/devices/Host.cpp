#include "devices/Host.h"
#include "network/Link.h"
#include "network/Network.h"
#include "core/SimulationEngine.h"
#include "core/Packet.h"
#include "utils/Logger.h"
#include "utils/StatisticsCollector.h"

Host::Host(const std::string& id, const std::string& mac, const std::string& ip)
    : Node(id, mac, ip) {}

void Host::sendPacket(const std::string& dstIP, const std::string& data) {
    Logger::getInstance().log("Host " + id_ + " [" + ipAddress_ +
                              "] отправляет пакет -> " + dstIP);
    StatisticsCollector::getInstance().recordSent(ipAddress_, dstIP);

    if (link_) {
        link_->transmit(id_, dstIP, data);
    }
}

void Host::receivePacket(const std::string& srcIP, const std::string& data) {
    Logger::getInstance().log("Host " + id_ + " [" + ipAddress_ +
                              "] получил пакет от " + srcIP + ": \"" + data + "\"");
    StatisticsCollector::getInstance().recordDelivered(srcIP, ipAddress_);
}

void Host::sendRealPacket(const std::string& dstIP, const std::string& dstMAC,
                         const std::string& data) {
    if (!link_) {
        Logger::getInstance().log("ERROR: Host " + id_ + " не подключен к каналу!");
        return;
    }

    // Создаем пакет
    auto packet = std::make_shared<Packet>(macAddress_, dstMAC, ipAddress_, dstIP, data);
    
    Logger::getInstance().log("[" + std::to_string(SimulationEngine::getInstance().getCurrentTime()) +
                              " us] Host " + id_ + " создает пакет #" + 
                              std::to_string(packet->getSequenceNumber()) + 
                              " для отправки на " + dstIP);

    // Записываем статистику
    StatisticsCollector::getInstance().recordSent(ipAddress_, dstIP);

    // Отправляем пакет по каналу
    link_->transmitPacket(packet);
}

void Host::receiveRealPacket(const std::string& srcIP, const std::string& srcMAC,
                            const std::string& data) {
    Logger::getInstance().log("[" + std::to_string(SimulationEngine::getInstance().getCurrentTime()) +
                              " us] Host " + id_ + " получил пакет от " + srcIP +
                              " (" + srcMAC + "): \"" + data + "\"");
    
    StatisticsCollector::getInstance().recordDelivered(srcIP, ipAddress_);
}

void Host::enableTrafficGeneration(double interval, const std::string& targetIP) {
    trafficInterval_ = interval;
    trafficTarget_ = targetIP;
    
    // Найдем MAC адрес целевого хоста
    Node* targetNode = Network::getInstance().findNodeByIP(targetIP);
    if (targetNode) {
        trafficTargetMAC_ = targetNode->getMAC();
    } else {
        Logger::getInstance().log("WARNING: Целевой хост с IP " + targetIP + " не найден!");
        trafficTargetMAC_ = "FF:FF:FF:FF:FF:FF";  // Broadcast MAC
    }
    
    generating_ = true;
    scheduleNextPacket();
    
    Logger::getInstance().log("Host " + id_ + " начинает генерировать трафик к " +
                              targetIP + " с интервалом " + std::to_string(interval) + " us");
}

void Host::disableTrafficGeneration() {
    generating_ = false;
    Logger::getInstance().log("Host " + id_ + " прекратил генерацию трафика");
}

void Host::scheduleNextPacket() {
    if (!generating_) return;

    double nextTime = SimulationEngine::getInstance().getCurrentTime() + trafficInterval_;
    
    Event e;
    e.type = EventType::HOST_GENERATE;
    e.time = nextTime;
    e.sourceId = id_;
    e.callback = [this]() {
        if (generating_) {
            sendRealPacket(trafficTarget_, trafficTargetMAC_, 
                          "Traffic from " + id_ + " to " + trafficTarget_);
            scheduleNextPacket();
        }
    };
    
    SimulationEngine::getInstance().scheduleEvent(e);
}
