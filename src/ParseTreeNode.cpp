#include "ParseTreeNode.h"

ParseTreeNode::ParseTreeNode(const std::string& type, const std::string& value)
    : type(type), value(value) {}

ParseTreeNode::~ParseTreeNode() {
    for (auto child : children) {
        delete child;
    }
}