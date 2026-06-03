#include "network/Link.h"
#include "devices/Node.h"
#include "protocols/TransmissionStrategy.h"
#include "protocols/CSMACDStrategy.h"
#include "protocols/PointToPointStrategy.h"
#include "utils/Logger.h"

Link::Link(const std::string& id, double bandwidthMbps, double lengthMeters, LinkType type)
    : id_(id), bandwidthMbps_(bandwidthMbps), lengthMeters_(lengthMeters), type_(type) {
    if (type_ == LinkType::SHARED_BUS) {
        strategy_ = std::make_unique<CSMACDStrategy>();
    } else {
        strategy_ = std::make_unique<PointToPointStrategy>();
    }
}

// Явный деструктор в .cpp, где TransmissionStrategy уже полностью определён
Link::~Link() = default;

void Link::connectNode(Node* node) {
    nodes_.push_back(node);
    Logger::getInstance().log("Узел " + node->getId() + " подключён к каналу " + id_);
}

void Link::transmit(const std::string& srcId, const std::string& dstId, const std::string& data) {
    Logger::getInstance().log("Канал " + id_ + ": передача от " + srcId + " -> " + dstId);
    if (strategy_) {
        strategy_->transmit(data, this);
    }

    // Доставка получателю
    for (auto* node : nodes_) {
        if (node->getIP() == dstId || node->getId() == dstId) {
            node->receivePacket(srcId, data);
            break;
        }
    }
}

bool Link::isBusy() const { return busy_; }
void Link::setBusy(bool busy) { busy_ = busy; }
std::vector<Node*>& Link::getNodes() { return nodes_; }
std::string Link::getId() const { return id_; }
double Link::getBandwidth() const { return bandwidthMbps_; }

double Link::getPropagationDelay() const {
    return lengthMeters_ / 200.0;
}

LinkType Link::getType() const { return type_; }

void Link::setStrategy(std::unique_ptr<TransmissionStrategy> strategy) {
    strategy_ = std::move(strategy);
}