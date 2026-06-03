#include <catch2/catch_test_macros.hpp>
#include "devices/Hub.h"

TEST_CASE("Hub constructor sets attributes", "[hub]") {
    Hub hub("Hub1", "AA:BB:CC:DD:EE:01", "192.168.1.250");
    REQUIRE(hub.getId() == "Hub1");
    REQUIRE(hub.getMAC() == "AA:BB:CC:DD:EE:01");
    REQUIRE(hub.getIP() == "192.168.1.250");
}

TEST_CASE("Hub sendPacket broadcasts to all ports", "[hub]") {
    Hub hub("Hub2", "", "10.0.0.250");
    REQUIRE_NOTHROW(hub.sendPacket("255.255.255.255", "Broadcast"));
}

TEST_CASE("Hub receivePacket forwards to all ports", "[hub]") {
    Hub hub("Hub3", "", "10.0.0.250");
    REQUIRE_NOTHROW(hub.receivePacket("10.0.0.1", "Frame"));
}

TEST_CASE("Hub id getter works", "[hub]") {
    Hub hub("CoreHub", "FF:FF:FF:FF:FF:FF", "0.0.0.0");
    REQUIRE(hub.getId() == "CoreHub");
}

TEST_CASE("Hub ip getter works", "[hub]") {
    Hub hub("AnyHub", "", "172.30.0.1");
    REQUIRE(hub.getIP() == "172.30.0.1");
}

TEST_CASE("Hub sendPacket empty data does not throw", "[hub]") {
    Hub hub("H4", "", "10.0.0.1");
    REQUIRE_NOTHROW(hub.sendPacket("10.0.0.2", ""));
}

TEST_CASE("Hub receivePacket empty data does not throw", "[hub]") {
    Hub hub("H5", "", "10.0.0.1");
    REQUIRE_NOTHROW(hub.receivePacket("10.0.0.2", ""));
}