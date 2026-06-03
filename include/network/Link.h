#pragma once
#include <string>
#include <vector>
#include <memory>

class Node;
class TransmissionStrategy;

enum class LinkType {
    SHARED_BUS,
    POINT_TO_POINT
};

class Link {
public:
    Link(const std::string& id, double bandwidthMbps, double lengthMeters, LinkType type);
    ~Link();  // <<< Явно объявляем деструктор

    void connectNode(Node* node);
    void transmit(const std::string& srcId, const std::string& dstId, const std::string& data);

    bool isBusy() const;
    void setBusy(bool busy);
    std::vector<Node*>& getNodes();

    std::string getId() const;
    double getBandwidth() const;
    double getPropagationDelay() const;
    LinkType getType() const;

    void setStrategy(std::unique_ptr<TransmissionStrategy> strategy);

private:
    std::string id_;
    double bandwidthMbps_;
    double lengthMeters_;
    LinkType type_;
    std::vector<Node*> nodes_;
    bool busy_ = false;
    std::unique_ptr<TransmissionStrategy> strategy_;
};