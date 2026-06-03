#pragma once
#include <string>

class ReportExporter {
public:
    static bool exportToCSV(const std::string& filename);
    static bool exportToText(const std::string& filename);
    static std::string generateReport();
};