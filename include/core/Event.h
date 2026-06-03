#pragma once
#include <string>
#include <functional>

enum class EventType {
    TRANSMIT_START,
    TRANSMIT_END,
    COLLISION,
    JAM_START,
    JAM_END,
    BACKOFF_TIMER,
    DELIVERY,
    HOST_GENERATE
};

struct Event {
    EventType type;
    double time;
    std::string sourceId;
    std::string targetId;
    std::string data;
    std::function<void()> callback;

    bool operator<(const Event& other) const {
        return time > other.time;
    }
};