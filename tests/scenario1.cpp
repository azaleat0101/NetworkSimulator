#include <catch2/catch_test_macros.hpp>
#include "network/Network.h"
#include "devices/Host.h"
#include "network/Link.h"
#include "utils/Logger.h"
#include <sstream>
#include <iostream>

TEST_CASE("Scenario 1: Host-to-Host transmission", "[scenario]") {
    Network::getInstance().clear();
    Logger::getInstance().clear();
    
    // Создаём два хоста
    auto host1 = std::make_shared<Host>("Alice", "AA:01", "192.168.1.10");
    auto host2 = std::make_shared<Host>("Bob", "AA:02", "192.168.1.20");
    
    // Создаём канал
    auto link = std::make_shared<Link>("Ethernet", 100, 50, LinkType::SHARED_BUS);
    
    // Подключаем хосты к каналу
    host1->connectToLink(link.get());
    host2->connectToLink(link.get());
    
    // Добавляем в сеть
    Network::getInstance().addNode(host1);
    Network::getInstance().addNode(host2);
    Network::getInstance().addLink(link);
    
    // Проверяем, что узлы добавлены
    REQUIRE(Network::getInstance().getNodes().size() == 2);
    REQUIRE(Network::getInstance().getLinks().size() == 1);
    
    // Отправляем пакет
    std::ostringstream oss;
    auto* old = std::cout.rdbuf(oss.rdbuf());
    
    host1->sendPacket("192.168.1.20", "Hello Bob!");
    
    std::cout.rdbuf(old);
    
    // Проверяем логи
    auto logs = Logger::getInstance().getLogs();
    REQUIRE_FALSE(logs.empty());
    
    // Ищем сообщение о передаче
    bool foundSend = false;
    for (auto& log : logs) {
        if (log.find("192.168.1.20") != std::string::npos) {
            foundSend = true;
            break;
        }
    }
    REQUIRE(foundSend);
}