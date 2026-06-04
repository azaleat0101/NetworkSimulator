#include "network/Link.h"
#include "core/Packet.h"
#include "devices/Node.h"
#include "protocols/TransmissionStrategy.h"
#include "utils/Logger.h"
#include "core/SimulationEngine.h"
#include <iostream>
#include <iomanip>

Link::Link(const std::string& id, double bandwidthMbps, double lengthMeters, LinkType type)
    : id_(id), bandwidthMbps_(bandwidthMbps), lengthMeters_(lengthMeters), type_(type),
      creationTime_(SimulationEngine::getInstance().getCurrentTime()) {}

Link::~Link() = default;

void Link::connectNode(Node* node) {
    nodes_.push_back(node);
    Logger::getInstance().log("  Узел " + node->getId() + " подключен к каналу " + id_);
}

std::vector<Node*>& Link::getNodes() {
    return nodes_;
}

void Link::transmitPacket(std::shared_ptr<Packet> packet) {
    if (!packet) return;

    double now = SimulationEngine::getInstance().getCurrentTime();
    
    // Добавляем пакет в очередь
    packetQueue_.push(packet);
    packet->setState(PacketState::QUEUED);
    
    Logger::getInstance().log("[" + std::to_string(now) + " us] Пакет #" + 
                              std::to_string(packet->getSequenceNumber()) +
                              " добавлен в очередь канала " + id_ + 
                              " (" + std::to_string(packetQueue_.size()) + " в очереди)");

    // Рассчитываем время передачи
    double txDelay = getTransmissionDelay(packet->getSize());
    
    // Если канал свободен, сразу начинаем передачу
    if (!busy_) {
        busy_ = true;
        packet->setState(PacketState::TRANSMITTING);
        
        Logger::getInstance().log("[" + std::to_string(now) + " us] Передача пакета #" +
                                  std::to_string(packet->getSequenceNumber()) +
                                  " на канале " + id_ + " (время: " + 
                                  std::to_string(txDelay) + " us)");

        // Планируем событие завершения передачи
        Event transmitEndEvent;
        transmitEndEvent.type = EventType::TRANSMIT_END;
        transmitEndEvent.time = now + txDelay;
        transmitEndEvent.sourceId = id_;
        transmitEndEvent.targetId = id_;
        transmitEndEvent.callback = [this, packet]() {
            double currentTime = SimulationEngine::getInstance().getCurrentTime();
            
            // Пакет успешно передан, добавляем время доставки
            double propagationDelay = getPropagationDelay();
            double deliveryTime = currentTime + propagationDelay;
            
            packet->setState(PacketState::DELIVERED);
            packet->setDeliveryTime(deliveryTime);
            totalTransmitted_++;
            totalTransmissionTime_ += (currentTime - packet->getCreationTime());
            
            Logger::getInstance().log("[" + std::to_string(currentTime) + " us] Пакет #" +
                                      std::to_string(packet->getSequenceNumber()) +
                                      " передан (доставка в " + std::to_string(deliveryTime) + " us)");
            
            // Переходим к следующему пакету в очереди
            if (!packetQueue_.empty()) {
                packetQueue_.pop();
                if (!packetQueue_.empty()) {
                    auto nextPacket = packetQueue_.front();
                    transmitPacket(nextPacket);
                } else {
                    busy_ = false;
                }
            } else {
                busy_ = false;
            }
        };
        
        SimulationEngine::getInstance().scheduleEvent(transmitEndEvent);
    }
}

std::shared_ptr<Packet> Link::receivePacket() {
    if (packetQueue_.empty()) return nullptr;
    auto packet = packetQueue_.front();
    packetQueue_.pop();
    return packet;
}

void Link::transmit(const std::string& srcId, const std::string& dstId, const std::string& data) {
    // Legacy method - kept for backward compatibility
    Logger::getInstance().log("  Канал " + id_ + ": передача от " + srcId + " к " + dstId);
    if (strategy_) {
        strategy_->transmit(data, this);
    }
}

bool Link::isBusy() const {
    return busy_;
}

void Link::setBusy(bool busy) {
    busy_ = busy;
}

size_t Link::getQueueSize() const {
    return packetQueue_.size();
}

std::string Link::getId() const {
    return id_;
}

double Link::getBandwidth() const {
    return bandwidthMbps_;
}

double Link::getPropagationDelay() const {
    // Пропускная способность = скорость света в среде
    // Время распространения = расстояние / скорость
    double delayPerKm = 1000.0 / SPEED_OF_LIGHT;  // микросекунды на км
    return (lengthMeters_ / 1000.0) * delayPerKm;  // в микросекундах
}

double Link::getTransmissionDelay(uint32_t packetSize) const {
    // Время передачи = размер пакета / полоса пропускания
    // Результат в микросекундах
    // Размер в битах / полоса в Мбит/сек = время в микросекундах
    uint32_t sizeInBits = packetSize * 8;
    return (sizeInBits / bandwidthMbps_);  // в микросекундах
}

LinkType Link::getType() const {
    return type_;
}

void Link::setStrategy(std::unique_ptr<TransmissionStrategy> strategy) {
    strategy_ = std::move(strategy);
}

TransmissionStrategy* Link::getStrategy() const {
    return strategy_.get();
}

int Link::getTotalTransmitted() const {
    return totalTransmitted_;
}

int Link::getTotalCollisions() const {
    return totalCollisions_;
}

double Link::getUtilization() const {
    double elapsedTime = SimulationEngine::getInstance().getCurrentTime() - creationTime_;
    if (elapsedTime == 0) return 0.0;
    return (totalTransmissionTime_ / elapsedTime) * 100.0;  // в процентах
}
