#include "utils/StatisticsCollector.h"
#include "core/SimulationEngine.h"

StatisticsCollector& StatisticsCollector::getInstance() {
    static StatisticsCollector instance;
    return instance;
}

void StatisticsCollector::recordSent(const std::string& srcIP, const std::string& dstIP) {
    totalSent_++;
    pendingSent_[srcIP + "->" + dstIP] = SimulationEngine::getInstance().getCurrentTime();
}

void StatisticsCollector::recordCollision(const std::string& srcIP) {
    totalCollisions_++;
    PacketRecord rec;
    rec.time = SimulationEngine::getInstance().getCurrentTime();
    rec.srcIP = srcIP;
    rec.collision = true;
    rec.delivered = false;
    rec.delay = 0;
    records_.push_back(rec);
}

void StatisticsCollector::recordDelivered(const std::string& srcIP, const std::string& dstIP) {
    totalDelivered_++;
    double now = SimulationEngine::getInstance().getCurrentTime();
    double delay = 0;

    std::string key = srcIP + "->" + dstIP;
    if (pendingSent_.find(key) != pendingSent_.end()) {
        delay = now - pendingSent_[key];
        pendingSent_.erase(key);
    }

    PacketRecord rec;
    rec.time = now;
    rec.srcIP = srcIP;
    rec.dstIP = dstIP;
    rec.delivered = true;
    rec.collision = false;
    rec.delay = delay;
    records_.push_back(rec);
}

int StatisticsCollector::getTotalSent() const { return totalSent_; }
int StatisticsCollector::getTotalDelivered() const { return totalDelivered_; }
int StatisticsCollector::getTotalCollisions() const { return totalCollisions_; }

double StatisticsCollector::getAvgDelay() const {
    double sum = 0;
    int count = 0;
    for (auto& r : records_) {
        if (r.delivered && r.delay > 0) {
            sum += r.delay;
            count++;
        }
    }
    return count > 0 ? sum / count : 0;
}

double StatisticsCollector::getLossRate() const {
    return totalSent_ > 0 ? 100.0 * (totalSent_ - totalDelivered_) / totalSent_ : 0;
}

std::vector<PacketRecord> StatisticsCollector::getAllRecords() const {
    return records_;
}

void StatisticsCollector::clear() {
    records_.clear();
    pendingSent_.clear();
    totalSent_ = 0;
    totalDelivered_ = 0;
    totalCollisions_ = 0;
}