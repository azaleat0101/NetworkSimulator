#pragma once
#include "devices/Node.h"

class Host : public Node {
public:
    Host(const std::string& id, const std::string& mac, const std::string& ip);

    void sendPacket(const std::string& dstIP, const std::string& data) override;
    void receivePacket(const std::string& srcIP, const std::string& data) override;

    void enableTrafficGeneration(double interval, const std::string& targetIP);
    void disableTrafficGeneration();

private:
    double trafficInterval_ = 0.0;
    std::string trafficTarget_;
    bool generating_ = false;
    void scheduleNextPacket();
};