#pragma once
#include "core/Event.h"
#include <queue>
#include <vector>

class ISimulationObserver;

class SimulationEngine {
public:
    static SimulationEngine& getInstance();

    void scheduleEvent(Event event);
    double getCurrentTime() const;
    void run(double maxTime = 1000.0);
    void stop();
    void step();
    bool isRunning() const;
    void setSpeed(double speed);

    void subscribe(ISimulationObserver* observer);
    void notifyFrameSent(const std::string& info);
    void notifyCollision(const std::string& info);

private:
    SimulationEngine() = default;
    std::priority_queue<Event> eventQueue_;
    std::vector<ISimulationObserver*> observers_;
    double currentTime_ = 0.0;
    double speed_ = 1.0;
    bool running_ = false;
};