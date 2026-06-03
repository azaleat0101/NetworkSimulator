#pragma once
#include <string>

class Link;

class Node {
public:
    Node(const std::string& id, const std::string& mac, const std::string& ip);
    virtual ~Node() = default;

    virtual void sendPacket(const std::string& dstIP, const std::string& data) = 0;
    virtual void receivePacket(const std::string& srcIP, const std::string& data) = 0;

    std::string getId() const;
    std::string getMAC() const;
    std::string getIP() const;

    void connectToLink(Link* link);
    Link* getLink() const;

protected:
    std::string id_;
    std::string macAddress_;
    std::string ipAddress_;
    Link* link_ = nullptr;
};