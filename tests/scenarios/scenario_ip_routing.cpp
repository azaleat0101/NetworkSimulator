#include <catch2/catch_test_macros.hpp>
#include "network/Network.h"
#include "devices/Host.h"
#include "devices/Router.h"
#include "network/Link.h"

TEST_CASE("Scenario: IP Routing", "[scenario]") {
    Network::getInstance().clear();
    
    auto hostA = std::make_shared<Host>("HostA", "AA:0A", "192.168.1.10");
    auto hostB = std::make_shared<Host>("HostB", "AA:0B", "192.168.2.20");
    auto router = std::make_shared<Router>("Router1", "AA:R1", "192.168.1.1");
    
    RoutingEntry entry{"192.168.2.0", "255.255.255.0", "192.168.2.1", "eth1"};
    router->addRoute(entry);
    
    auto link1 = std::make_shared<Link>("LAN1", 100, 50, LinkType::POINT_TO_POINT);
    hostA->connectToLink(link1.get());
    router->connectToLink(link1.get());
    
    Network::getInstance().addNode(hostA);
    Network::getInstance().addNode(hostB);
    Network::getInstance().addNode(router);
    
    REQUIRE_NOTHROW(hostA->sendPacket("192.168.2.20", "Routing Test"));
}
