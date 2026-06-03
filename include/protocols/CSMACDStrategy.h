#pragma once
#include "protocols/TransmissionStrategy.h"

class CSMACDStrategy : public TransmissionStrategy {
public:
    void transmit(const std::string& data, Link* link) override;

private:
    double calculateBackoffTime(int attempt);
    int attemptCounter_ = 0;
};