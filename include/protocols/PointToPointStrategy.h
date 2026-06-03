#pragma once
#include "protocols/TransmissionStrategy.h"

class PointToPointStrategy : public TransmissionStrategy {
public:
    void transmit(const std::string& data, Link* link) override;
};