#include <catch2/catch_test_macros.hpp>
#include "utils/ConfigLoader.h"
#include "network/Network.h"
#include "devices/Host.h"
#include "network/Link.h"
#include <fstream>

TEST_CASE("ConfigLoader saveToJson creates file", "[config]") {
    Network::getInstance().clear();
    auto host = std::make_shared<Host>("Test", "AA:01", "10.0.0.1");
    Network::getInstance().addNode(host);

    REQUIRE(ConfigLoader::saveToJson("test_output.json"));

    std::ifstream file("test_output.json");
    REQUIRE(file.good());
    file.close();
    std::remove("test_output.json");
}

TEST_CASE("ConfigLoader loadFromJson file not found", "[config]") {
    bool result = ConfigLoader::loadFromJson("nonexistent.json");
    REQUIRE(result == false);
}

TEST_CASE("ConfigLoader loadFromJson valid file", "[config]") {
    // Создаём корректный JSON для загрузки
    std::ofstream config("valid_test.json");
    config << "{\n";
    config << "  \"network\": {\n";
    config << "    \"nodes\": [\n";
    config << "      {\"type\": \"host\", \"id\": \"Node1\", \"mac\": \"AA:01\", \"ip\": \"10.0.0.1\"},\n";
    config << "      {\"type\": \"host\", \"id\": \"Node2\", \"mac\": \"AA:02\", \"ip\": \"10.0.0.2\"}\n";
    config << "    ],\n";
    config << "    \"links\": [\n";
    config << "      {\"type\": \"link\", \"id\": \"Link1\", \"bandwidth\": 100, \"length\": 50, \"linkType\": \"shared_bus\", \"nodes\": \"Node1,Node2\"}\n";
    config << "    ]\n";
    config << "  }\n";
    config << "}\n";
    config.close();

    Network::getInstance().clear();
    bool loaded = ConfigLoader::loadFromJson("valid_test.json");
    REQUIRE(loaded == true);
    REQUIRE(Network::getInstance().getNodes().size() == 2);
    REQUIRE(Network::getInstance().getLinks().size() == 1);

    // Проверяем узлы
    REQUIRE(Network::getInstance().findNodeById("Node1") != nullptr);
    REQUIRE(Network::getInstance().findNodeById("Node2") != nullptr);
    REQUIRE(Network::getInstance().findNodeByIP("10.0.0.1") != nullptr);
    REQUIRE(Network::getInstance().findNodeByIP("10.0.0.2") != nullptr);

    std::remove("valid_test.json");
}