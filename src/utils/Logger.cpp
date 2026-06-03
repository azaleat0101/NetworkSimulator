#include "utils/Logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << "[" << std::put_time(&tm, "%H:%M:%S") << "] " << message;
    logs_.push_back(oss.str());
    std::cout << oss.str() << std::endl;
}

void Logger::error(const std::string& message) {
    log("ERROR: " + message);
}

std::vector<std::string> Logger::getLogs() const {
    return logs_;
}

void Logger::clear() {
    logs_.clear();
}