#include "utils/ConfigLoader.h"
#include "network/Network.h"
#include "devices/Host.h"
#include "devices/Router.h"
#include "devices/Hub.h"
#include "network/Link.h"
#include "utils/Logger.h"
#include <fstream>
#include <sstream>

static std::string getJsonValue(const std::string& line, const std::string& key) {
    std::string search = "\"" + key + "\":";
    size_t pos = line.find(search);
    if (pos == std::string::npos) return "";

    pos += search.length();
    while (pos < line.length() && (line[pos] == ' ' || line[pos] == '\"')) pos++;

    size_t end = line.find_first_of("\",\n\r}", pos);
    if (end == std::string::npos) return "";

    return line.substr(pos, end - pos);
}

bool ConfigLoader::loadFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().error("Не удалось открыть файл: " + filename);
        return false;
    }

    // Очищаем текущую сеть
    Network::getInstance().clear();

    std::string line;
    std::vector<std::string> pendingConnections; // nodeId -> linkId

    while (std::getline(file, line)) {
        if (line.find("\"type\":") != std::string::npos) {
            std::string type = getJsonValue(line, "type");

            if (type == "host") {
                std::string id = getJsonValue(line, "id");
                std::string mac = getJsonValue(line, "mac");
                std::string ip = getJsonValue(line, "ip");
                Network::getInstance().addNode(std::make_shared<Host>(id, mac, ip));
            }
            else if (type == "router") {
                std::string id = getJsonValue(line, "id");
                std::string mac = getJsonValue(line, "mac");
                std::string ip = getJsonValue(line, "ip");
                auto router = std::make_shared<Router>(id, mac, ip);

                std::string routes = getJsonValue(line, "routes");
                if (!routes.empty()) {
                    std::istringstream iss(routes);
                    std::string route;
                    while (std::getline(iss, route, ';')) {
                        std::istringstream rss(route);
                        std::string net, mask, hop, iface;
                        std::getline(rss, net, ',');
                        std::getline(rss, mask, ',');
                        std::getline(rss, hop, ',');
                        std::getline(rss, iface, ',');
                        router->addRoute({net, mask, hop, iface});
                    }
                }
                Network::getInstance().addNode(router);
            }
            else if (type == "hub") {
                std::string id = getJsonValue(line, "id");
                std::string mac = getJsonValue(line, "mac");
                std::string ip = getJsonValue(line, "ip");
                Network::getInstance().addNode(std::make_shared<Hub>(id, mac, ip));
            }
            else if (type == "link") {
                std::string id = getJsonValue(line, "id");
                double bandwidth = std::stod(getJsonValue(line, "bandwidth"));
                double length = std::stod(getJsonValue(line, "length"));
                std::string linkType = getJsonValue(line, "linkType");

                LinkType lt = (linkType == "shared_bus") ? LinkType::SHARED_BUS : LinkType::POINT_TO_POINT;
                auto link = std::make_shared<Link>(id, bandwidth, length, lt);

                std::string nodeIds = getJsonValue(line, "nodes");
                std::istringstream nss(nodeIds);
                std::string nodeId;
                while (std::getline(nss, nodeId, ',')) {
                    Node* node = Network::getInstance().findNodeById(nodeId);
                    if (node) {
                        node->connectToLink(link.get());
                    }
                }

                Network::getInstance().addLink(link);
            }
        }
    }

    Logger::getInstance().log("Конфигурация загружена из " + filename);
    return true;
}

bool ConfigLoader::saveToJson(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().error("Не удалось сохранить в " + filename);
        return false;
    }

    auto& network = Network::getInstance();

    file << "{\n  \"network\": {\n    \"nodes\": [\n";
    auto nodes = network.getNodes();
    for (size_t i = 0; i < nodes.size(); i++) {
        file << "      {\n";
        file << "        \"type\": \"host\",\n";  // <<< Добавлен type
        file << "        \"id\": \"" << nodes[i]->getId() << "\",\n";
        file << "        \"mac\": \"" << nodes[i]->getMAC() << "\",\n";
        file << "        \"ip\": \"" << nodes[i]->getIP() << "\"\n";
        file << "      }" << (i < nodes.size() - 1 ? "," : "") << "\n";
    }

    file << "    ],\n    \"links\": [\n";
    auto links = network.getLinks();
    for (size_t i = 0; i < links.size(); i++) {
        file << "      {\n";
        file << "        \"type\": \"link\",\n";  // <<< Добавлен type
        file << "        \"id\": \"" << links[i]->getId() << "\",\n";
        file << "        \"bandwidth\": " << links[i]->getBandwidth() << ",\n";
        file << "        \"length\": 100,\n";
        file << "        \"linkType\": \"" << (links[i]->getType() == LinkType::SHARED_BUS ? "shared_bus" : "point_to_point") << "\",\n";
        file << "        \"nodes\": \"";
        auto lnodes = links[i]->getNodes();
        for (size_t j = 0; j < lnodes.size(); j++) {
            file << lnodes[j]->getId();
            if (j < lnodes.size() - 1) file << ",";
        }
        file << "\"\n      }" << (i < links.size() - 1 ? "," : "") << "\n";
    }
    file << "    ]\n  }\n}\n";

    Logger::getInstance().log("Конфигурация сохранена в " + filename);
    return true;
}