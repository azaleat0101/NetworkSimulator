#pragma once
#include <vector>
#include <memory>
#include <string>

class Node;
class Link;

class Network {
public:
    static Network& getInstance();

    void addNode(std::shared_ptr<Node> node);
    void addLink(std::shared_ptr<Link> link);

    std::vector<std::shared_ptr<Node>> getNodes() const;
    std::vector<std::shared_ptr<Link>> getLinks() const;

    Node* findNodeByIP(const std::string& ip) const;
    Node* findNodeById(const std::string& id) const;

    void printTopology() const;
    void clear();

private:
    Network() = default;
    std::vector<std::shared_ptr<Node>> nodes_;
    std::vector<std::shared_ptr<Link>> links_;
};