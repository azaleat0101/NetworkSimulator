#include "protocols/CSMACDStrategy.h"
#include "network/Link.h"
#include "core/SimulationEngine.h"
#include "utils/Logger.h"
#include "utils/StatisticsCollector.h"
#include <random>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>

void CSMACDStrategy::transmit(const std::string& data, Link* link) {
    double now = SimulationEngine::getInstance().getCurrentTime();

    // 1. Проверка занятости канала
    if (link->isBusy()) {
        Logger::getInstance().log("  CSMA/CD: Канал занят, ожидание...");
        attemptCounter_++;
        double backoff = calculateBackoffTime(attemptCounter_);
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(backoff / 1000)));
        attemptCounter_ = 0;
        return;
    }

    // 2. Захват канала
    link->setBusy(true);
    Logger::getInstance().log("  CSMA/CD: Канал захвачен, передача начата");

    // 3. Имитация передачи
    double delay = link->getPropagationDelay();
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(delay / 100)));

    // 4. Проверка коллизии (10% шанс)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 10);

    if (dis(gen) == 1) {
        Logger::getInstance().log("  CSMA/CD: ⚡ КОЛЛИЗИЯ обнаружена!");
        StatisticsCollector::getInstance().recordCollision("unknown");
        SimulationEngine::getInstance().notifyCollision("Collision on link " + link->getId());
    } else {
        Logger::getInstance().log("  CSMA/CD: Передача завершена успешно");
        SimulationEngine::getInstance().notifyFrameSent("Frame sent on link " + link->getId());
    }

    // 5. Освобождение канала
    link->setBusy(false);
}

double CSMACDStrategy::calculateBackoffTime(int attempt) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    int k = std::min(attempt, 10);
    std::uniform_int_distribution<> dis(0, static_cast<int>(std::pow(2, k)) - 1);
    return dis(gen) * 51.2; // микросекунды (слот Ethernet)
}