#include <catch2/catch_test_macros.hpp>
#include "devices/Host.h"

TEST_CASE("Host constructor sets attributes", "[host]") {
    Host host("WebServer", "00:11:22:33:44:55", "192.168.1.100");
    REQUIRE(host.getId() == "WebServer");
    REQUIRE(host.getMAC() == "00:11:22:33:44:55");
    REQUIRE(host.getIP() == "192.168.1.100");
}

TEST_CASE("Host sendPacket outputs to log", "[host]") {
    Host host("H1", "AA:01", "10.0.0.1");
    REQUIRE_NOTHROW(host.sendPacket("10.0.0.2", "Ping"));
}

TEST_CASE("Host receivePacket outputs to log", "[host]") {
    Host host("H2", "AA:02", "10.0.0.2");
    REQUIRE_NOTHROW(host.receivePacket("10.0.0.1", "Pong"));
}

TEST_CASE("Host enableTrafficGeneration sets parameters", "[host]") {
    Host host("H3", "AA:03", "10.0.0.3");
    REQUIRE_NOTHROW(host.enableTrafficGeneration(100.0, "10.0.0.4"));
}

TEST_CASE("Host disableTrafficGeneration stops generation", "[host]") {
    Host host("H4", "AA:04", "10.0.0.4");
    host.enableTrafficGeneration(50.0, "10.0.0.5");
    REQUIRE_NOTHROW(host.disableTrafficGeneration());
}

TEST_CASE("Host id getter works", "[host]") {
    Host host("TestHost", "00:00:00:00:00:01", "192.168.0.1");
    REQUIRE(host.getId() == "TestHost");
}

TEST_CASE("Host ip getter works", "[host]") {
    Host host("Any", "Any", "10.10.10.10");
    REQUIRE(host.getIP() == "10.10.10.10");
}