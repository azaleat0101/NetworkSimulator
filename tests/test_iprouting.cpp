#include <catch2/catch_test_macros.hpp>
#include "protocols/IPRouting.h"

TEST_CASE("IPRouting matchNetwork exact match", "[iprouting]") {
    REQUIRE(IPRouting::matchNetwork("192.168.1.10", "192.168.1.0", "255.255.255.0"));
}

TEST_CASE("IPRouting matchNetwork different subnet", "[iprouting]") {
    REQUIRE_FALSE(IPRouting::matchNetwork("192.168.2.10", "192.168.1.0", "255.255.255.0"));
}

TEST_CASE("IPRouting matchNetwork broadcast address", "[iprouting]") {
    REQUIRE(IPRouting::matchNetwork("10.0.0.255", "10.0.0.0", "255.255.255.0"));
}

TEST_CASE("IPRouting matchNetwork with different masks", "[iprouting]") {
    REQUIRE(IPRouting::matchNetwork("172.16.5.5", "172.16.0.0", "255.255.0.0"));
    REQUIRE_FALSE(IPRouting::matchNetwork("172.17.5.5", "172.16.0.0", "255.255.0.0"));
}

TEST_CASE("IPRouting resolveNextHop finds route", "[iprouting]") {
    std::vector<RouteEntry> table = {
        {"192.168.1.0", "255.255.255.0", "192.168.1.1"},
        {"192.168.2.0", "255.255.255.0", "192.168.2.1"},
        {"10.0.0.0", "255.0.0.0", "10.0.0.1"}
    };
    REQUIRE(IPRouting::resolveNextHop("192.168.2.50", table) == "192.168.2.1");
}

TEST_CASE("IPRouting resolveNextHop returns default for unknown", "[iprouting]") {
    std::vector<RouteEntry> table = {
        {"192.168.1.0", "255.255.255.0", "192.168.1.1"}
    };
    REQUIRE(IPRouting::resolveNextHop("10.10.10.10", table) == "0.0.0.0");
}

TEST_CASE("IPRouting resolveNextHop empty table", "[iprouting]") {
    std::vector<RouteEntry> table;
    REQUIRE(IPRouting::resolveNextHop("192.168.1.1", table) == "0.0.0.0");
}