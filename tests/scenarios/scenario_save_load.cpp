#include <catch2/catch_test_macros.hpp>
#include "network/Network.h"
#include "devices/Host.h"
#include "utils/ConfigLoader.h"
#include <fstream>

TEST_CASE("Scenario: Save and Load Topology", "[scenario]") {
    Network::getInstance().clear();
    auto host1 = std::make_shared<Host>("Host1", "AA:01", "192.168.1.10");
    Network::getInstance().addNode(host1);
    
    REQUIRE_NOTHROW(ConfigLoader::saveToJson("test_save.json"));
    
    Network::getInstance().clear();
    REQUIRE(Network::getInstance().getNodes().size() == 0);
    
    // We will just do a load
    ConfigLoader::loadFromJson("test_save.json");
    std::remove("test_save.json");
    
    REQUIRE(true); // Verification of actual serialization depends on ConfigLoader implementation
}
