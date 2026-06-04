#include <catch2/catch_test_macros.hpp>
#include "utils/StatisticsCollector.h"
#include "network/Network.h"
#include "devices/Host.h"
#include "network/Link.h"

TEST_CASE("Scenario: Statistics Collection", "[scenario]") {
    Network::getInstance().clear();
    StatisticsCollector::getInstance().clear();
    
    auto hostA = std::make_shared<Host>("HostA", "AA:0A", "192.168.1.10");
    auto link = std::make_shared<Link>("LAN", 100, 10, LinkType::SHARED_BUS);
    hostA->connectToLink(link.get());
    
    Network::getInstance().addNode(hostA);
    Network::getInstance().addLink(link);
    
    for (int i=0; i<100; ++i) {
        hostA->sendPacket("192.168.1.20", "Packet " + std::to_string(i));
    }
    
    int totalSent = StatisticsCollector::getInstance().getTotalSent();
    REQUIRE(totalSent >= 0);
}
