#include "core/SimulationEngine.h"
#include "core/Packet.h"
#include "network/Network.h"
#include "devices/Host.h"
#include "devices/Router.h"
#include "devices/Hub.h"
#include "network/Link.h"
#include "utils/Logger.h"
#include "utils/StatisticsCollector.h"
#include "utils/ConsoleRenderer.h"
#include "utils/ReportExporter.h"
#include "utils/ConfigLoader.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>

/**
 * Д��монстрация реальной передачи пакетов по сети
 * с полным моделированием задержек и времени передачи
 */

void buildRealNetworkDemo() {
    auto& net = Network::getInstance();
    
    Logger::getInstance().log("=== Построение демонстрационной сети с реальной передачей пакетов ===");

    // Создаем хосты в первой подсети (192.168.1.x)
    auto host1 = std::make_shared<Host>("Host1", "AA:BB:CC:01:01:01", "192.168.1.10");
    auto host2 = std::make_shared<Host>("Host2", "AA:BB:CC:01:01:02", "192.168.1.20");
    
    // Создаем хосты во второй подсети (192.168.2.x)
    auto host3 = std::make_shared<Host>("Host3", "AA:BB:CC:02:01:01", "192.168.2.10");
    auto host4 = std::make_shared<Host>("Host4", "AA:BB:CC:02:01:02", "192.168.2.20");
    
    // Создаем маршрутизатор (соединяет обе подсети)
    auto router = std::make_shared<Router>("Router1", "AA:BB:CC:FF:FF:FF", "192.168.1.1");

    // Настраиваем маршруты в маршрутизаторе
    router->addRoute({"192.168.1.0", "255.255.255.0", "192.168.1.1", "eth0"});
    router->addRoute({"192.168.2.0", "255.255.255.0", "192.168.2.1", "eth1"});

    // Создаем каналы связи
    // Канал 1: 100 Мбит/с, 10 метров (Ethernet подсеть 1)
    auto link1 = std::make_shared<Link>("Link1", 100.0, 10.0, LinkType::POINT_TO_POINT);
    
    // Канал 2: 100 Мбит/с, 100 метров (Ethernet подсеть 2)
    auto link2 = std::make_shared<Link>("Link2", 100.0, 100.0, LinkType::POINT_TO_POINT);
    
    // Канал 3: 1000 Мбит/с, 1000 метров (магистральной канал между роутерами)
    auto link3 = std::make_shared<Link>("Link3", 1000.0, 1000.0, LinkType::POINT_TO_POINT);

    // Подключаем хосты к соответствующим каналам
    host1->connectToLink(link1.get());
    host2->connectToLink(link1.get());
    
    host3->connectToLink(link2.get());
    host4->connectToLink(link2.get());
    
    router->connectToLink(link3.get());

    // Добавляем все узлы и каналы в сеть
    net.addNode(host1);
    net.addNode(host2);
    net.addNode(host3);
    net.addNode(host4);
    net.addNode(router);
    
    net.addLink(link1);
    net.addLink(link2);
    net.addLink(link3);

    Logger::getInstance().log("Сеть построена: 4 хоста + 1 маршрутизатор + 3 канала");
}

void demonstrateRealTransmission() {
    Logger::getInstance().log("\n=== ДЕМОНСТРАЦИЯ РЕАЛЬНОЙ ПЕРЕДАЧИ ПАКЕТОВ ===\n");
    
    auto& engine = SimulationEngine::getInstance();
    auto& net = Network::getInstance();
    
    // Получаем хосты
    Host* h1 = dynamic_cast<Host*>(net.findNodeByIP("192.168.1.10"));
    Host* h2 = dynamic_cast<Host*>(net.findNodeByIP("192.168.1.20"));
    Host* h3 = dynamic_cast<Host*>(net.findNodeByIP("192.168.2.10"));

    if (!h1 || !h2 || !h3) {
        Logger::getInstance().log("ERROR: Не удалось найти хосты!");
        return;
    }

    // Отправляем одиночные пакеты с паузами для наблюдения
    Logger::getInstance().log("\n[Тест 1] Host1 отправляет пакет Host2 (в одной подсети)");
    h1->sendRealPacket("192.168.1.20", "AA:BB:CC:01:01:02", "Hello from Host1 to Host2!");

    Logger::getInstance().log("\n[Тест 2] Host1 отправляет пакет Host3 (в другой подсети)");
    h1->sendRealPacket("192.168.2.10", "AA:BB:CC:02:01:01", "Hello from Host1 to Host3!");

    Logger::getInstance().log("\n[Тест 3] Host2 отправляет пакет Host1");
    h2->sendRealPacket("192.168.1.10", "AA:BB:CC:01:01:01", "Reply from Host2!");

    Logger::getInstance().log("\n[Тест 4] Включаем генерацию трафика Host3 -> Host2 с интервалом 500 us");
    h3->enableTrafficGeneration(500.0, "192.168.1.20");
}

void printNetworkStatistics() {
    auto& net = Network::getInstance();
    auto& stats = StatisticsCollector::getInstance();
    
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              СТАТИСТИКА СИМУЛЯЦИИ СЕТИ                      ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    
    std::cout << "║ Всего отправлено пакетов:       " << std::setw(30) << std::left 
              << std::to_string(stats.getTotalSent()) << " ║\n";
    std::cout << "║ Успешно доставлено пакетов:    " << std::setw(30) << std::left 
              << std::to_string(stats.getTotalDelivered()) << " ║\n";
    std::cout << "║ Потеряно пакетов:              " << std::setw(30) << std::left 
              << std::to_string(stats.getTotalSent() - stats.getTotalDelivered()) << " ║\n";
    std::cout << "║ Обнаружено коллизий:           " << std::setw(30) << std::left 
              << std::to_string(stats.getTotalCollisions()) << " ║\n";
    
    if (stats.getTotalDelivered() > 0) {
        std::cout << "║ Средняя задержка доставки:    " << std::setw(30) << std::left 
                  << std::to_string(stats.getAvgDelay()) + " us" << " ║\n";
    }
    
    std::cout << "║ Коэффициент потерь пакетов:    " << std::setw(29) << std::left 
              << std::to_string(stats.getLossRate() * 100) + "%" << " ║\n";
    
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    std::cout << "║                    ИНФОРМАЦИЯ ПО КАНАЛАМ                     ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    
    for (const auto& link : net.getLinks()) {
        std::cout << "║ " << std::setw(20) << link->getId() << ": " 
                  << std::setw(8) << link->getBandwidth() << " Мбит/с | "
                  << std::setw(8) << link->getTotalTransmitted() << " пакетов | "
                  << std::setw(6) << link->getUtilization() << "% нагр. ║\n";
    }
    
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";
}

int main() {
    ConsoleRenderer::clearScreen();
    ConsoleRenderer::renderHeader();

    try {
        // Строим сеть
        buildRealNetworkDemo();
        
        // Выводим топологию
        Network::getInstance().printTopology();
        
        // Показываем таблицу маршрутизации
        Router* router = dynamic_cast<Router*>(Network::getInstance().findNodeByIP("192.168.1.1"));
        if (router) {
            router->printRoutingTable();
        }

        // Демонстрируем реальную передачу
        demonstrateRealTransmission();

        // Устанавливаем скорость симуляции
        SimulationEngine::getInstance().setSpeed(1.0);  // Реальное время

        std::cout << "\nНажмите Enter для запуска симуляции...";
        std::cin.get();

        // Запускаем симуляцию на 10000 микросекунд
        Logger::getInstance().log("Запуск симуляции...");
        SimulationEngine::getInstance().run(10000.0);

        // Выводим статистику
        printNetworkStatistics();

        std::cout << "\nСимуляция завершена. Нажмите Enter для выхода...";
        std::cin.get();

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
