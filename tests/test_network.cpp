#include <catch2/catch_test_macros.hpp>
#include "network/Network.h"
#include "devices/Host.h"
#include "devices/Router.h"
#include "network/Link.h"

TEST_CASE("Network Singleton returns same instance", "[network]") {
    auto& n1 = Network::getInstance();
    auto& n2 = Network::getInstance();
    REQUIRE(&n1 == &n2);
}

TEST_CASE("Network addNode adds node to topology", "[network]") {
    Network::getInstance().clear();
    auto host = std::make_shared<Host>("H1", "AA:01", "10.0.0.1");
    Network::getInstance().addNode(host);
    REQUIRE(Network::getInstance().getNodes().size() == 1);
}

TEST_CASE("Network addLink adds link to topology", "[network]") {
    Network::getInstance().clear();
    auto link = std::make_shared<Link>("L1", 10, 100, LinkType::SHARED_BUS);
    Network::getInstance().addLink(link);
    REQUIRE(Network::getInstance().getLinks().size() == 1);
}

TEST_CASE("Network findNodeByIP returns correct node", "[network]") {
    Network::getInstance().clear();
    auto host = std::make_shared<Host>("Server", "BB:01", "192.168.1.100");
    Network::getInstance().addNode(host);
    
    Node* found = Network::getInstance().findNodeByIP("192.168.1.100");
    REQUIRE(found != nullptr);
    REQUIRE(found->getId() == "Server");
}

TEST_CASE("Network findNodeByIP returns nullptr for unknown IP", "[network]") {
    Network::getInstance().clear();
    REQUIRE(Network::getInstance().findNodeByIP("10.10.10.10") == nullptr);
}

TEST_CASE("Network findNodeById returns correct node", "[network]") {
    Network::getInstance().clear();
    auto host = std::make_shared<Host>("NodeX", "", "1.1.1.1");
    Network::getInstance().addNode(host);
    
    Node* found = Network::getInstance().findNodeById("NodeX");
    REQUIRE(found != nullptr);
    REQUIRE(found->getIP() == "1.1.1.1");
}

TEST_CASE("Network findNodeById returns nullptr for unknown ID", "[network]") {
    Network::getInstance().clear();
    REQUIRE(Network::getInstance().findNodeById("NonExistent") == nullptr);
}

TEST_CASE("Network clear removes all nodes and links", "[network]") {
    auto& net = Network::getInstance();
    net.clear();
    net.addNode(std::make_shared<Host>("H1", "", ""));
    net.addNode(std::make_shared<Host>("H2", "", ""));
    net.addLink(std::make_shared<Link>("L1", 10, 100, LinkType::SHARED_BUS));
    
    net.clear();
    REQUIRE(net.getNodes().empty());
    REQUIRE(net.getLinks().empty());
}

TEST_CASE("Network printTopology does not throw", "[network]") {
    Network::getInstance().clear();
    Network::getInstance().addNode(std::make_shared<Host>("H1", "", "10.0.0.1"));
    Network::getInstance().addNode(std::make_shared<Router>("R1", "", "10.0.0.254"));
    Network::getInstance().addLink(std::make_shared<Link>("Eth1", 10, 100, LinkType::SHARED_BUS));
    REQUIRE_NOTHROW(Network::getInstance().printTopology());
}