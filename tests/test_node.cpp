#include <catch2/catch_test_macros.hpp>
#include "devices/Node.h"

// Конкретный класс для тестирования абстрактного Node
class TestNode : public Node {
public:
    TestNode(const std::string& id, const std::string& mac, const std::string& ip)
        : Node(id, mac, ip) {}
    
    void sendPacket(const std::string& dstIP, const std::string& data) override {
        lastSentDst = dstIP;
        lastSentData = data;
    }
    
    void receivePacket(const std::string& srcIP, const std::string& data) override {
        lastReceivedSrc = srcIP;
        lastReceivedData = data;
    }
    
    std::string lastSentDst;
    std::string lastSentData;
    std::string lastReceivedSrc;
    std::string lastReceivedData;
};

TEST_CASE("Node constructor sets attributes correctly", "[node]") {
    TestNode node("N1", "AA:BB:CC:DD:EE:FF", "192.168.1.100");
    REQUIRE(node.getId() == "N1");
    REQUIRE(node.getMAC() == "AA:BB:CC:DD:EE:FF");
    REQUIRE(node.getIP() == "192.168.1.100");
}

TEST_CASE("Node getId returns correct value", "[node]") {
    TestNode node("Router5", "00:00:00:00:00:05", "10.0.0.5");
    REQUIRE(node.getId() == "Router5");
}

TEST_CASE("Node getMAC returns correct value", "[node]") {
    TestNode node("X", "11:22:33:44:55:66", "10.0.0.1");
    REQUIRE(node.getMAC() == "11:22:33:44:55:66");
}

TEST_CASE("Node getIP returns correct value", "[node]") {
    TestNode node("Y", "AA:BB:CC:DD:EE:FF", "172.16.0.1");
    REQUIRE(node.getIP() == "172.16.0.1");
}

TEST_CASE("Node getLink returns nullptr initially", "[node]") {
    TestNode node("Z", "", "1.1.1.1");
    REQUIRE(node.getLink() == nullptr);
}

TEST_CASE("Node sendPacket stores parameters", "[node]") {
    TestNode node("H1", "", "10.0.0.1");
    node.sendPacket("10.0.0.2", "Test data");
    REQUIRE(node.lastSentDst == "10.0.0.2");
    REQUIRE(node.lastSentData == "Test data");
}

TEST_CASE("Node receivePacket stores parameters", "[node]") {
    TestNode node("H2", "", "10.0.0.2");
    node.receivePacket("10.0.0.1", "Response");
    REQUIRE(node.lastReceivedSrc == "10.0.0.1");
    REQUIRE(node.lastReceivedData == "Response");
}