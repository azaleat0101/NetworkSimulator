#include "core/SimulationEngine.h"
#include "utils/Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

// Интерфейс наблюдателя
class ISimulationObserver {
public:
    virtual ~ISimulationObserver() = default;
    virtual void onFrameSent(const std::string& info) = 0;
    virtual void onCollision(const std::string& info) = 0;
};

SimulationEngine& SimulationEngine::getInstance() {
    static SimulationEngine instance;
    return instance;
}

void SimulationEngine::scheduleEvent(Event event) {
    eventQueue_.push(event);
}

double SimulationEngine::getCurrentTime() const {
    return currentTime_;
}

void SimulationEngine::run(double maxTime) {
    running_ = true;
    Logger::getInstance().log("=== Симуляция запущена ===");

    while (running_ && !eventQueue_.empty() && currentTime_ < maxTime) {
        Event event = eventQueue_.top();
        eventQueue_.pop();

        if (event.time > maxTime) break;

        currentTime_ = event.time;

        if (event.callback) {
            event.callback();
        }

        if (speed_ < 100) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(100 / speed_)));
        }
    }

    Logger::getInstance().log("=== Симуляция завершена (время: " +
                               std::to_string(currentTime_) + " мкс) ===");
    running_ = false;
}

void SimulationEngine::stop() {
    running_ = false;
}

void SimulationEngine::step() {
    if (!eventQueue_.empty()) {
        Event event = eventQueue_.top();
        eventQueue_.pop();
        currentTime_ = event.time;
        if (event.callback) event.callback();
    }
}

bool SimulationEngine::isRunning() const {
    return running_;
}

void SimulationEngine::setSpeed(double speed) {
    speed_ = speed;
}

void SimulationEngine::subscribe(ISimulationObserver* observer) {
    observers_.push_back(observer);
}

void SimulationEngine::notifyFrameSent(const std::string& info) {
    for (auto* obs : observers_)
        obs->onFrameSent(info);
}

void SimulationEngine::notifyCollision(const std::string& info) {
    for (auto* obs : observers_)
        obs->onCollision(info);
}