#pragma once
#include <string>

class Link;

class TransmissionStrategy {
public:
    virtual ~TransmissionStrategy() = default;
    virtual void transmit(const std::string& data, Link* link) = 0;
};