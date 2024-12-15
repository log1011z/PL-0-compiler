#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <string>
#include <vector>

class ParseTreeNode {
public:
    std::string type;
    std::string value;
    std::vector<ParseTreeNode*> children;

    ParseTreeNode(const std::string& type, const std::string& value);
    ~ParseTreeNode();
};

#endif // PARSETREENODE_H