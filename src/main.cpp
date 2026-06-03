#include "core/SimulationEngine.h"
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

void buildDemoNetwork() {
    auto& net = Network::getInstance();

    auto host1 = std::make_shared<Host>("Host1", "AA:01", "192.168.1.10");
    auto host2 = std::make_shared<Host>("Host2", "AA:02", "192.168.1.20");
    auto host3 = std::make_shared<Host>("Host3", "AA:03", "192.168.2.10");
    auto router = std::make_shared<Router>("Router1", "AA:FF", "192.168.1.1");

    router->addRoute({"192.168.1.0", "255.255.255.0", "192.168.1.1", "eth0"});
    router->addRoute({"192.168.2.0", "255.255.255.0", "192.168.2.1", "eth1"});

    auto link1 = std::make_shared<Link>("Ethernet1", 10, 100, LinkType::SHARED_BUS);

    host1->connectToLink(link1.get());
    host2->connectToLink(link1.get());
    host3->connectToLink(link1.get());
    router->connectToLink(link1.get());

    net.addNode(host1);
    net.addNode(host2);
    net.addNode(host3);
    net.addNode(router);
    net.addLink(link1);
}

int main() {
    ConsoleRenderer::clearScreen();
    ConsoleRenderer::renderHeader();

    buildDemoNetwork();
    SimulationEngine::getInstance().setSpeed(10.0);

    int choice;
    do {
        std::cout << "\n--- МЕНЮ ---\n";
        std::cout << "1. Показать топологию\n";
        std::cout << "2. Host1 -> Host2\n";
        std::cout << "3. Host2 -> Host1\n";
        std::cout << "4. Host1 -> Host3 (через Router)\n";
        std::cout << "5. Показать статистику\n";
        std::cout << "6. Экспорт CSV\n";
        std::cout << "7. Экспорт TXT\n";
        std::cout << "8. Загрузить из JSON\n";
        std::cout << "9. Сохранить в JSON\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: Network::getInstance().printTopology(); break;
            case 2: {
                auto* h1 = Network::getInstance().findNodeById("Host1");
                if (h1) h1->sendPacket("192.168.1.20", "Hello Host2!");
                break;
            }
            case 3: {
                auto* h2 = Network::getInstance().findNodeById("Host2");
                if (h2) h2->sendPacket("192.168.1.10", "Hello Host1!");
                break;
            }
            case 4: {
                auto* h1 = Network::getInstance().findNodeById("Host1");
                if (h1) h1->sendPacket("192.168.2.10", "Packet via Router");
                break;
            }
            case 5: ConsoleRenderer::renderStats(); break;
            case 6: ReportExporter::exportToCSV("report.csv"); break;
            case 7: ReportExporter::exportToText("report.txt"); break;
            case 8: {
                ConfigLoader::loadFromJson("network.json");
                break;
            }
            case 9: ConfigLoader::saveToJson("network_out.json"); break;
            case 0: std::cout << "Выход.\n"; break;
            default: std::cout << "Неверный выбор.\n";
        }

        if (choice != 0) {
            std::cout << "\nНажмите Enter...";
            std::cin.get();
        }
    } while (choice != 0);

    return 0;
}