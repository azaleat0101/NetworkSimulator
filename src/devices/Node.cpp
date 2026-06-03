#include "devices/Node.h"
#include "network/Link.h"

Node::Node(const std::string& id, const std::string& mac, const std::string& ip)
    : id_(id), macAddress_(mac), ipAddress_(ip) {}

std::string Node::getId() const { return id_; }
std::string Node::getMAC() const { return macAddress_; }
std::string Node::getIP() const { return ipAddress_; }

void Node::connectToLink(Link* link) {
    link_ = link;
    link->connectNode(this);
}

Link* Node::getLink() const {
    return link_;
}