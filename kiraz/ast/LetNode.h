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
        return fmt::format("Let(n={}, t={}, i={})", 
                            m_name ? m_name->as_string() : "null",
                            m_type ? m_type->as_string() : "null",
                            m_initializer ? m_initializer->as_string() : "null");
    }

private:
    Node::Ptr m_name;        
    Node::Ptr m_type;         
    Node::Ptr m_initializer;  
};



} // namespace ast

#endif // KIRAZ_AST_LET_NODE_H
