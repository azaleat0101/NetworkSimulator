#include <catch2/catch_test_macros.hpp>
#include "utils/StatisticsCollector.h"

TEST_CASE("StatisticsCollector Singleton returns same instance", "[statistics]") {
    auto& s1 = StatisticsCollector::getInstance();
    auto& s2 = StatisticsCollector::getInstance();
    REQUIRE(&s1 == &s2);
}

TEST_CASE("StatisticsCollector initial state is zero", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    REQUIRE(StatisticsCollector::getInstance().getTotalSent() == 0);
    REQUIRE(StatisticsCollector::getInstance().getTotalDelivered() == 0);
    REQUIRE(StatisticsCollector::getInstance().getTotalCollisions() == 0);
}

TEST_CASE("StatisticsCollector recordSent increments counter", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    StatisticsCollector::getInstance().recordSent("192.168.1.1", "192.168.1.2");
    REQUIRE(StatisticsCollector::getInstance().getTotalSent() == 1);
}

TEST_CASE("StatisticsCollector recordDelivered increments counter", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    StatisticsCollector::getInstance().recordSent("10.0.0.1", "10.0.0.2");
    StatisticsCollector::getInstance().recordDelivered("10.0.0.1", "10.0.0.2");
    REQUIRE(StatisticsCollector::getInstance().getTotalDelivered() == 1);
}

TEST_CASE("StatisticsCollector recordCollision increments counter", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    StatisticsCollector::getInstance().recordCollision("192.168.1.1");
    StatisticsCollector::getInstance().recordCollision("192.168.1.2");
    REQUIRE(StatisticsCollector::getInstance().getTotalCollisions() == 2);
}

TEST_CASE("StatisticsCollector getLossRate returns correct value", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    StatisticsCollector::getInstance().recordSent("A", "B");
    StatisticsCollector::getInstance().recordSent("A", "C");
    StatisticsCollector::getInstance().recordSent("A", "D");
    StatisticsCollector::getInstance().recordDelivered("A", "B");
    // 3 отправлено, 1 доставлено = 66.67% потерь
    double lossRate = StatisticsCollector::getInstance().getLossRate();
    REQUIRE(lossRate > 66.0);
    REQUIRE(lossRate < 67.0);
}

TEST_CASE("StatisticsCollector getAllRecords returns all packets", "[statistics]") {
    StatisticsCollector::getInstance().clear();
    StatisticsCollector::getInstance().recordSent("1.1.1.1", "2.2.2.2");
    StatisticsCollector::getInstance().recordDelivered("1.1.1.1", "2.2.2.2");
    StatisticsCollector::getInstance().recordCollision("3.3.3.3");
    
    auto records = StatisticsCollector::getInstance().getAllRecords();
    REQUIRE(records.size() == 2); // recordSent не создаёт запись, только доставка и коллизия
}