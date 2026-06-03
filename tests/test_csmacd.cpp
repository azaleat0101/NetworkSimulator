#include <catch2/catch_test_macros.hpp>
#include "protocols/CSMACDStrategy.h"
#include "network/Link.h"

TEST_CASE("CSMACDStrategy transmit on shared bus", "[csmacd]") {
    CSMACDStrategy strategy;
    Link link("TestLink", 10, 100, LinkType::SHARED_BUS);
    REQUIRE_NOTHROW(strategy.transmit("Test data", &link));
}

TEST_CASE("CSMACDStrategy transmit on busy link", "[csmacd]") {
    CSMACDStrategy strategy;
    Link link("BusyLink", 100, 50, LinkType::SHARED_BUS);
    link.setBusy(true);
    REQUIRE_NOTHROW(strategy.transmit("Data on busy", &link));
}

TEST_CASE("CSMACDStrategy transmit empty data", "[csmacd]") {
    CSMACDStrategy strategy;
    Link link("EmptyLink", 10, 200, LinkType::SHARED_BUS);
    REQUIRE_NOTHROW(strategy.transmit("", &link));
}

TEST_CASE("CSMACDStrategy transmit long data", "[csmacd]") {
    CSMACDStrategy strategy;
    Link link("LongLink", 1000, 10, LinkType::SHARED_BUS);
    std::string longData(10000, 'A');
    REQUIRE_NOTHROW(strategy.transmit(longData, &link));
}

TEST_CASE("CSMACDStrategy multiple transmissions", "[csmacd]") {
    CSMACDStrategy strategy;
    Link link("MultiLink", 100, 100, LinkType::SHARED_BUS);
    for (int i = 0; i < 10; i++) {
        REQUIRE_NOTHROW(strategy.transmit("Packet " + std::to_string(i), &link));
    }
}