#include <catch2/catch_test_macros.hpp>
#include "devices/Router.h"

TEST_CASE("Router constructor sets attributes", "[router]") {
    Router router("R1", "AA:BB:CC:DD:EE:FF", "192.168.1.1");
    REQUIRE(router.getId() == "R1");
    REQUIRE(router.getMAC() == "AA:BB:CC:DD:EE:FF");
    REQUIRE(router.getIP() == "192.168.1.1");
}

TEST_CASE("Router addRoute adds entry to table", "[router]") {
    Router router("R2", "", "10.0.0.1");
    RoutingEntry entry = {"192.168.1.0", "255.255.255.0", "192.168.1.1", "eth0"};
    REQUIRE_NOTHROW(router.addRoute(entry));
}

TEST_CASE("Router sendPacket does not throw", "[router]") {
    Router router("R3", "", "10.0.0.1");
    router.addRoute({"10.0.1.0", "255.255.255.0", "10.0.1.254", "eth1"});
    REQUIRE_NOTHROW(router.sendPacket("10.0.1.5", "Test"));
}

TEST_CASE("Router receivePacket does not throw", "[router]") {
    Router router("R4", "", "10.0.0.1");
    REQUIRE_NOTHROW(router.receivePacket("192.168.1.10", "Data"));
}

TEST_CASE("Router printRoutingTable does not throw", "[router]") {
    Router router("R5", "", "10.0.0.1");
    router.addRoute({"192.168.0.0", "255.255.0.0", "192.168.0.1", "eth0"});
    router.addRoute({"10.0.0.0", "255.0.0.0", "10.0.0.1", "eth1"});
    REQUIRE_NOTHROW(router.printRoutingTable());
}

TEST_CASE("Router handles empty routing table", "[router]") {
    Router router("R6", "", "10.0.0.1");
    REQUIRE_NOTHROW(router.sendPacket("10.0.1.1", "No routes"));
    REQUIRE_NOTHROW(router.printRoutingTable());
}

TEST_CASE("Router multiple routes work", "[router]") {
    Router router("R7", "", "172.16.0.1");
    router.addRoute({"172.16.1.0", "255.255.255.0", "172.16.1.254", "eth0"});
    router.addRoute({"172.16.2.0", "255.255.255.0", "172.16.2.254", "eth1"});
    router.addRoute({"172.16.3.0", "255.255.255.0", "172.16.3.254", "eth2"});
    REQUIRE_NOTHROW(router.sendPacket("172.16.2.10", "Multi-route test"));
}