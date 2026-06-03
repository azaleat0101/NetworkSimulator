#include "devices/Hub.h"
#include "utils/Logger.h"

Hub::Hub(const std::string& id, const std::string& mac, const std::string& ip)
    : Node(id, mac, ip) {}

void Hub::sendPacket(const std::string& dstIP, const std::string& data) {
    Logger::getInstance().log("Hub " + id_ + " рассылает пакет на все порты");
    for (auto* port : ports_) {
        port->receivePacket(ipAddress_, data);
    }
}

void Hub::receivePacket(const std::string& srcIP, const std::string& data) {
    Logger::getInstance().log("Hub " + id_ + " получил пакет от " + srcIP);
    // Пересылаем на все порты кроме входящего
    sendPacket("", data);
}