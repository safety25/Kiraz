#ifndef KIRAZ_AST_FUNCNODE_H
#define KIRAZ_AST_FUNCNODE_H

#include <kiraz/Node.h>
#include <kiraz/Token.h>
#include <vector>
#include <memory>

namespace ast {

class ArgNode : public Node {
public:
    ArgNode(Node::Ptr name, Node::Ptr type)
        : Node(IDENTIFIER), m_name(std::move(name)), m_type(std::move(type)) {}
    
    std::string as_string() const override {
        return fmt::format("FArg(n={}, t={})", 
                           m_name ? m_name->as_string() : "null",
                           m_type ? m_type->as_string() : "null");
    }

private:
    Node::Ptr m_name;
    Node::Ptr m_type;
};


class FuncArgs : public Node {
public:
    FuncArgs() : Node(OP_LPAREN) {}
    
    void add_argument(Node::Ptr arg) {
        m_args.push_back(std::move(arg));
    }

    std::string as_string() const override {
        std::string result = "FuncArgs([";
        for (size_t i = 0; i < m_args.size(); ++i) {
            result += m_args[i]->as_string();
            if (i < m_args.size() - 1) result += ", ";
        }
        result += "])";
        return result;
    }

private:
    std::vector<Node::Ptr> m_args;
};




} // namespace ast

#endif // KIRAZ_AST_FUNCNODE_H
