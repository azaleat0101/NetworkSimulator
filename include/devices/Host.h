#pragma once
#include "devices/Node.h"

class Host : public Node {
public:
    Host(const std::string& id, const std::string& mac, const std::string& ip);

    void sendPacket(const std::string& dstIP, const std::string& data) override;
    void receivePacket(const std::string& srcIP, const std::string& data) override;

    // Traffic generation
    void enableTrafficGeneration(double interval, const std::string& targetIP);
    void disableTrafficGeneration();

    // Real packet methods
    void sendRealPacket(const std::string& dstIP, const std::string& dstMAC, 
                       const std::string& data);
    void receiveRealPacket(const std::string& srcIP, const std::string& srcMAC,
                          const std::string& data);

private:
    double trafficInterval_ = 0.0;
    std::string trafficTarget_;
    std::string trafficTargetMAC_;
    bool generating_ = false;
    void scheduleNextPacket();
};
