#include "network/Network.h"
#include "devices/Node.h"
#include "network/Link.h"
#include "utils/Logger.h"
#include <iostream>
#include <iomanip>

Network& Network::getInstance() {
    static Network instance;
    return instance;
}

void Network::addNode(std::shared_ptr<Node> node) {
    nodes_.push_back(node);
    Logger::getInstance().log("Добавлен узел: " + node->getId() + " [" + node->getIP() + "]");
}

void Network::addLink(std::shared_ptr<Link> link) {
    links_.push_back(link);
    Logger::getInstance().log("Добавлен канал: " + link->getId());
}

std::vector<std::shared_ptr<Node>> Network::getNodes() const {
    return nodes_;
}

std::vector<std::shared_ptr<Link>> Network::getLinks() const {
    return links_;
}

Node* Network::findNodeByIP(const std::string& ip) const {
    for (auto& node : nodes_) {
        if (node->getIP() == ip) return node.get();
    }
    return nullptr;
}

Node* Network::findNodeById(const std::string& id) const {
    for (auto& node : nodes_) {
        if (node->getId() == id) return node.get();
    }
    return nullptr;
}

void Network::printTopology() const {
    std::cout << "\n╔═══════════════════ ТОПОЛОГИЯ СЕТИ ═══════════════════╗\n";
    std::cout << "║ Узлы:                                                 ║\n";
    for (auto& node : nodes_) {
        std::cout << "║  " << std::setw(10) << std::left << node->getId()
                  << " | IP: " << std::setw(15) << node->getIP()
                  << " | MAC: " << node->getMAC() << " ║\n";
    }
    std::cout << "║                                                       ║\n";
    std::cout << "║ Каналы:                                               ║\n";
    for (auto& link : links_) {
        std::cout << "║  " << std::setw(10) << link->getId()
                  << " | Тип: " << (link->getType() == LinkType::SHARED_BUS ? "Шина" : "P2P")
                  << " | Узлов: " << link->getNodes().size() << " ║\n";
    }
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";
}

void Network::clear() {
    nodes_.clear();
    links_.clear();
}