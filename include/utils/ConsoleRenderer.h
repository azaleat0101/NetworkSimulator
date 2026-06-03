#pragma once

class Network;

class ConsoleRenderer {
public:
    static void clearScreen();
    static void renderTopology(const Network& network);
    static void renderStats();
    static void renderHeader();
};