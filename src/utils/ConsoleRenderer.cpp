#include "utils/ConsoleRenderer.h"
#include "utils/StatisticsCollector.h"
#include "network/Network.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void ConsoleRenderer::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderer::renderHeader() {
    std::cout << "╔══════════════════════════════════════════════════╗\n";
    std::cout << "║     СИМУЛЯТОР КОМПЬЮТЕРНОЙ СЕТИ v1.0             ║\n";
    std::cout << "║     CSMA/CD + IP-маршрутизация                   ║\n";
    std::cout << "╚══════════════════════════════════════════════════╝\n";
}

void ConsoleRenderer::renderTopology(const Network& network) {
    network.printTopology();
}

void ConsoleRenderer::renderStats() {
    auto& stats = StatisticsCollector::getInstance();
    std::cout << "\n╔══════════════════ СТАТИСТИКА ══════════════════╗\n";
    std::cout << "║ Отправлено:    " << std::setw(10) << stats.getTotalSent() << "                  ║\n";
    std::cout << "║ Доставлено:    " << std::setw(10) << stats.getTotalDelivered() << "                  ║\n";
    std::cout << "║ Коллизий:      " << std::setw(10) << stats.getTotalCollisions() << "                  ║\n";
    std::cout << "║ Потери:        " << std::setw(8) << std::fixed << std::setprecision(1)
              << stats.getLossRate() << "%                ║\n";
    std::cout << "║ Средняя задержка: " << std::setw(6) << std::fixed << std::setprecision(2)
              << stats.getAvgDelay() << " мкс            ║\n";
    std::cout << "╚════════════════════════════════════════════════╝\n";
}