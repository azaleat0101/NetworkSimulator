#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "utils/Logger.h"
#include <sstream>
#include <iostream>

TEST_CASE("Logger Singleton returns same instance", "[logger]") {
    Logger& l1 = Logger::getInstance();
    Logger& l2 = Logger::getInstance();
    REQUIRE(&l1 == &l2);
}

TEST_CASE("Logger log adds message to internal storage", "[logger]") {
    Logger::getInstance().clear();
    Logger::getInstance().log("Test message 1");
    auto logs = Logger::getInstance().getLogs();
    REQUIRE(logs.size() == 1);
    REQUIRE_THAT(logs[0], Catch::Matchers::ContainsSubstring("Test message 1"));
}

TEST_CASE("Logger log outputs to stdout", "[logger]") {
    std::ostringstream oss;
    auto* old = std::cout.rdbuf(oss.rdbuf());
    
    Logger::getInstance().log("Hello stdout");
    
    std::cout.rdbuf(old);
    REQUIRE_THAT(oss.str(), Catch::Matchers::ContainsSubstring("Hello stdout"));
}

TEST_CASE("Logger error adds ERROR prefix", "[logger]") {
    Logger::getInstance().clear();
    Logger::getInstance().error("Something went wrong");
    auto logs = Logger::getInstance().getLogs();
    REQUIRE(logs.size() == 1);
    REQUIRE_THAT(logs[0], Catch::Matchers::ContainsSubstring("ERROR"));
    REQUIRE_THAT(logs[0], Catch::Matchers::ContainsSubstring("Something went wrong"));
}

TEST_CASE("Logger clear removes all messages", "[logger]") {
    Logger::getInstance().log("Message 1");
    Logger::getInstance().log("Message 2");
    Logger::getInstance().log("Message 3");
    REQUIRE(Logger::getInstance().getLogs().size() == 3);
    
    Logger::getInstance().clear();
    REQUIRE(Logger::getInstance().getLogs().empty());
}

TEST_CASE("Logger handles empty message", "[logger]") {
    Logger::getInstance().clear();
    REQUIRE_NOTHROW(Logger::getInstance().log(""));
    auto logs = Logger::getInstance().getLogs();
    REQUIRE(logs.size() == 1);
}

TEST_CASE("Logger handles very long message", "[logger]") {
    Logger::getInstance().clear();
    std::string longMsg(10000, 'X');
    REQUIRE_NOTHROW(Logger::getInstance().log(longMsg));
    auto logs = Logger::getInstance().getLogs();
    REQUIRE(logs.size() == 1);
    REQUIRE(logs[0].find(longMsg) != std::string::npos);
}