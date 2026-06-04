#pragma once
#include <string>
#include <memory>

class Host;
class Router;
class Link;

/**
 * Интерактивный конструктор сетевой топологии
 * Позволяет пользователю создавать сеть через консольное меню
 */
class TopologyBuilder {
public:
    static TopologyBuilder& getInstance();

    // Основные операции
    void showMenu();
    void addHost();
    void addRouter();
    void addLink();
    void connectNodeToLink();
    void configureRouting();
    void viewTopology();
    void clearNetwork();
    
    // Сценарии
    void loadPredefinedScenario(int scenario);

private:
    TopologyBuilder() = default;
    
    void displayNodeList();
    void displayLinkList();
    
    std::string generateMAC();
    static int macCounter_;
};
