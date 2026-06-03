#include "utils/ReportExporter.h"
#include "utils/StatisticsCollector.h"
#include "utils/Logger.h"
#include <fstream>
#include <iomanip>
#include <sstream>

bool ReportExporter::exportToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().error("Не удалось открыть файл: " + filename);
        return false;
    }

    file << "Time,Source,Destination,Delivered,Collision,Delay\n";
    auto& stats = StatisticsCollector::getInstance();
    for (auto& rec : stats.getAllRecords()) {
        file << rec.time << ","
             << rec.srcIP << ","
             << rec.dstIP << ","
             << (rec.delivered ? "Yes" : "No") << ","
             << (rec.collision ? "Yes" : "No") << ","
             << rec.delay << "\n";
    }

    Logger::getInstance().log("Отчёт экспортирован в " + filename);
    return true;
}

bool ReportExporter::exportToText(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::getInstance().error("Не удалось открыть файл: " + filename);
        return false;
    }

    file << generateReport();
    Logger::getInstance().log("Отчёт экспортирован в " + filename);
    return true;
}

std::string ReportExporter::generateReport() {
    auto& stats = StatisticsCollector::getInstance();
    std::ostringstream oss;

    oss << "========================================\n";
    oss << "  ОТЧЁТ ПО СИМУЛЯЦИИ СЕТИ\n";
    oss << "========================================\n\n";
    oss << "Общая статистика:\n";
    oss << "  Отправлено пакетов:  " << stats.getTotalSent() << "\n";
    oss << "  Доставлено пакетов:  " << stats.getTotalDelivered() << "\n";
    oss << "  Потеряно пакетов:    " << (stats.getTotalSent() - stats.getTotalDelivered()) << "\n";
    oss << "  Коллизий:            " << stats.getTotalCollisions() << "\n";
    oss << "  Процент потерь:      " << std::fixed << std::setprecision(1) << stats.getLossRate() << "%\n";
    oss << "  Средняя задержка:    " << std::fixed << std::setprecision(2) << stats.getAvgDelay() << " мкс\n";

    return oss.str();
}