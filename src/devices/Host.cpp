#include "devices/Host.h"
#include "network/Link.h"
#include "core/SimulationEngine.h"
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

void Host::enableTrafficGeneration(double interval, const std::string& targetIP) {
    trafficInterval_ = interval;
    trafficTarget_ = targetIP;
    generating_ = true;
    scheduleNextPacket();
}

void Host::disableTrafficGeneration() {
    generating_ = false;
}

void Host::scheduleNextPacket() {
    if (!generating_) return;

    Event e;
    e.type = EventType::HOST_GENERATE;
    e.time = SimulationEngine::getInstance().getCurrentTime() + trafficInterval_;
    e.sourceId = id_;
    e.callback = [this]() {
        if (generating_) {
            sendPacket(trafficTarget_, "Traffic data from " + id_);
            scheduleNextPacket();
        }
    };
    SimulationEngine::getInstance().scheduleEvent(e);
}