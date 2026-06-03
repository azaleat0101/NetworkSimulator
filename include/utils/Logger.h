#pragma once
#include <string>
#include <vector>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& message);
    void error(const std::string& message);
    std::vector<std::string> getLogs() const;
    void clear();

private:
    Logger() = default;
    std::vector<std::string> logs_;
};