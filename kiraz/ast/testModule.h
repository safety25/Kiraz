#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>

namespace ast {

class Module : public Node {
public:
    Module(Node::Ptr root) : Node(-1), m_root(root) {}

    std::string as_string() const override {
        if (!m_root) {
        return ""; 
    }
        return fmt::format("Module([{}])", m_root->as_string());

    }

private:
    Node::Ptr m_root;
};

} 

#endif // KIRAZ_AST_MODULE_H