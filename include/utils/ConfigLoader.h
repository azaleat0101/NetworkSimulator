#pragma once
#include <string>

class Network;

class ConfigLoader {
public:
    static bool loadFromJson(const std::string& filename);
    static bool saveToJson(const std::string& filename);
};