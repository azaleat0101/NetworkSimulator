#include "protocols/PointToPointStrategy.h"
#include "network/Link.h"
#include "utils/Logger.h"
#include <iostream>

void PointToPointStrategy::transmit(const std::string& data, Link* link) {
    Logger::getInstance().log("  Point-to-Point: Прямая передача без коллизий");
    // В канале точка-точка коллизий быть не может
    double delay = link->getPropagationDelay();
    Logger::getInstance().log("  Point-to-Point: Задержка " + std::to_string(delay) + " мкс");
}