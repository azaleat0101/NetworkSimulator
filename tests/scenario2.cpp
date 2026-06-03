#include <catch2/catch_test_macros.hpp>
#include "utils/ConfigLoader.h"
#include "network/Network.h"
#include <fstream>

TEST_CASE("Scenario 2: Load and verify configuration", "[scenario]") {
    // Создаём тестовый JSON
    std::ofstream config("test_network.json");
    config << "{\n";
    config << "  \"network\": {\n";
    config << "    \"nodes\": [\n";
    config << "      {\"type\": \"host\", \"id\": \"PC1\", \"mac\": \"AA:01\", \"ip\": \"192.168.1.10\"},\n";
    config << "      {\"type\": \"host\", \"id\": \"PC2\", \"mac\": \"AA:02\", \"ip\": \"192.168.1.20\"},\n";
    config << "      {\"type\": \"router\", \"id\": \"R1\", \"mac\": \"FF:01\", \"ip\": \"192.168.1.1\", \"routes\": \"192.168.1.0,255.255.255.0,192.168.1.1,eth0\"}\n";
    config << "    ],\n";
    config << "    \"links\": [\n";
    config << "      {\"type\": \"link\", \"id\": \"LAN1\", \"bandwidth\": 100, \"length\": 50, \"linkType\": \"shared_bus\", \"nodes\": \"PC1,PC2,R1\"}\n";
    config << "    ]\n";
    config << "  }\n";
    config << "}\n";
    config.close();

    // Очищаем сеть и загружаем конфигурацию
    Network::getInstance().clear();
    bool success = ConfigLoader::loadFromJson("test_network.json");
    REQUIRE(success == true);

    // Проверяем узлы
    REQUIRE(Network::getInstance().getNodes().size() == 3);
    REQUIRE(Network::getInstance().getLinks().size() == 1);

    // Проверяем наличие конкретных узлов
    REQUIRE(Network::getInstance().findNodeById("PC1") != nullptr);
    REQUIRE(Network::getInstance().findNodeById("PC2") != nullptr);
    REQUIRE(Network::getInstance().findNodeById("R1") != nullptr);

    // Проверяем IP-адреса
    REQUIRE(Network::getInstance().findNodeByIP("192.168.1.10") != nullptr);
    REQUIRE(Network::getInstance().findNodeByIP("192.168.1.1") != nullptr);

    // Проверяем, что несуществующий IP возвращает nullptr
    REQUIRE(Network::getInstance().findNodeByIP("10.0.0.1") == nullptr);

    // Очистка
    std::remove("test_network.json");
}