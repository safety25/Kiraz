#ifndef KIRAZ_AST_LET_NODE_H
#define KIRAZ_AST_LET_NODE_H

#include <kiraz/Node.h>
#include <string>
#include <kiraz/Compiler.h>

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

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
        set_cur_symtab(st.get_cur_symtab());

        if (auto var_name = std::dynamic_pointer_cast<const ast::Identifier>(m_name)) {
            if (st.get_symbol(var_name->get_name())) {
                return set_error(fmt::format("Variable '{}' is already defined", var_name->get_name()));
            }

            st.add_symbol(var_name->get_name(), shared_from_this());
        } else {
            return set_error("LetNode name must be an identifier");
        }

        return shared_from_this(); 
    }

private:
    Node::Ptr m_name;        
    Node::Ptr m_type;         
    Node::Ptr m_initializer;  
};

}  // namespace ast

#endif // KIRAZ_AST_LET_NODE_H
