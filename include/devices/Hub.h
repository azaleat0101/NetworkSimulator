#pragma once
#include "devices/Node.h"
#include <vector>

class Hub : public Node {
public:
    Hub(const std::string& id, const std::string& mac, const std::string& ip);

    void sendPacket(const std::string& dstIP, const std::string& data) override;
    void receivePacket(const std::string& srcIP, const std::string& data) override;

private:
    std::vector<Node*> ports_;
};