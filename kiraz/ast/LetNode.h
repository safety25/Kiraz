#ifndef KIRAZ_AST_LET_NODE_H
#define KIRAZ_AST_LET_NODE_H

#include <kiraz/Node.h>
#include <string>

namespace ast {

class LetNode : public Node {
public:
    LetNode(Node::Ptr name, Node::Ptr type = nullptr, Node::Ptr initializer = nullptr)
        : Node(KW_LET), m_name(name), m_type(type), m_initializer(initializer) {}

    std::string as_string() const override {
        std::string result = fmt::format("Let(n={}", m_name->as_string());

    if (m_type) {
        result += fmt::format(", t={}", m_type->as_string());
    }
    
    if (m_initializer) {
        result += fmt::format(", i={}", m_initializer->as_string());
    }

    result += ")";
    return result;
    }

private:
    Node::Ptr m_name;        
    Node::Ptr m_type;         
    Node::Ptr m_initializer;  
};



}

#endif // KIRAZ_AST_LET_NODE_H
