#include <catch2/catch_test_macros.hpp>
#include "network/Link.h"
#include "devices/Host.h"

TEST_CASE("Link constructor sets parameters", "[link]") {
    Link link("L1", 100.0, 50.0, LinkType::SHARED_BUS);
    REQUIRE(link.getId() == "L1");
    REQUIRE(link.getBandwidth() == 100.0);
    REQUIRE(link.getType() == LinkType::SHARED_BUS);
}

TEST_CASE("Link connectNode adds node", "[link]") {
    Link link("L2", 10, 100, LinkType::POINT_TO_POINT);
    Host host("H1", "AA:01", "10.0.0.1");
    REQUIRE_NOTHROW(link.connectNode(&host));
    REQUIRE(link.getNodes().size() == 1);
}

TEST_CASE("Link connectNode multiple nodes", "[link]") {
    Link link("L3", 1000, 10, LinkType::SHARED_BUS);
    Host h1("A", "", "1.1.1.1");
    Host h2("B", "", "1.1.1.2");
    Host h3("C", "", "1.1.1.3");
    link.connectNode(&h1);
    link.connectNode(&h2);
    link.connectNode(&h3);
    REQUIRE(link.getNodes().size() == 3);
}

TEST_CASE("Link isBusy initially false", "[link]") {
    Link link("L4", 10, 100, LinkType::SHARED_BUS);
    REQUIRE_FALSE(link.isBusy());
}

TEST_CASE("Link setBusy changes state", "[link]") {
    Link link("L5", 10, 100, LinkType::SHARED_BUS);
    link.setBusy(true);
    REQUIRE(link.isBusy());
    link.setBusy(false);
    REQUIRE_FALSE(link.isBusy());
}

TEST_CASE("Link getPropagationDelay calculates correctly", "[link]") {
    Link link("L6", 10, 200, LinkType::POINT_TO_POINT);
    double delay = link.getPropagationDelay();
    REQUIRE(delay > 0.0);
    REQUIRE(delay < 10.0); // должно быть около 1 мкс для 200м
}

TEST_CASE("Link transmit does not throw with empty nodes", "[link]") {
    Link link("L7", 10, 100, LinkType::SHARED_BUS);
    REQUIRE_NOTHROW(link.transmit("A", "B", "test"));
}