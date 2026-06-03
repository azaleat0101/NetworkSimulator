#pragma once
#include <string>
#include <vector>
#include <map>

struct PacketRecord {
    double time;
    std::string srcIP;
    std::string dstIP;
    double delay;
    bool delivered;
    bool collision;
};

class StatisticsCollector {
public:
    static StatisticsCollector& getInstance();

    void recordSent(const std::string& srcIP, const std::string& dstIP);
    void recordCollision(const std::string& srcIP);
    void recordDelivered(const std::string& srcIP, const std::string& dstIP);

    int getTotalSent() const;
    int getTotalDelivered() const;
    int getTotalCollisions() const;
    double getAvgDelay() const;
    double getLossRate() const;

    std::vector<PacketRecord> getAllRecords() const;
    void clear();

private:
    StatisticsCollector() = default;
    std::vector<PacketRecord> records_;
    std::map<std::string, double> pendingSent_;
    int totalSent_ = 0;
    int totalDelivered_ = 0;
    int totalCollisions_ = 0;
};