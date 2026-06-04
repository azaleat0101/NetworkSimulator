#include <catch2/catch_test_macros.hpp>
#include "network/Network.h"
#include "devices/Host.h"
#include <string>

TEST_CASE("Scenario: Headless 200 nodes", "[scenario]") {
    Network::getInstance().clear();
    
    for (int i=0; i<200; ++i) {
        std::string ip = "10.0.0." + std::to_string(i+1);
        auto host = std::make_shared<Host>("Host" + std::to_string(i), "AA:BB:" + std::to_string(i), ip);
        Network::getInstance().addNode(host);
    }
    
    REQUIRE(Network::getInstance().getNodes().size() == 200);
}
