#ifndef KIRAZ_AST_LET_NODE_H
#define KIRAZ_AST_LET_NODE_H

#include <kiraz/Node.h>
#include <string>
#include <kiraz/Compiler.h>
#include <kiraz/ast/KeyNodes.h>

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
            
            return set_error(fmt::format("Identifier '{}' is already in symtab", var_name->get_name()));
            }  else {
                st.add_symbol(var_name->get_name(), shared_from_this());
            }
        if (isupper(var_name->get_name()[0])) {
                return set_error(fmt::format("Variable name '{}' can not start with an uppercase letter", var_name->get_name()));
            } else {
                st.add_symbol(var_name->get_name(), shared_from_this());
            }

        }

        if (m_type) {
            if (auto type_name = std::dynamic_pointer_cast<const ast::Identifier>(m_type)) {
                if (!st.get_symbol(type_name->get_name())) {
                    return set_error(fmt::format("Type '{}' not found", type_name->get_name()));
                }
            } else {
                return set_error("LetNode type must be an identifier");
            }
        }

        if (m_initializer) {
                if (auto while_node = std::dynamic_pointer_cast<const ast::WhileNode>(m_initializer)) {
                    return nullptr;
                }
                
                else if (auto if_node = std::dynamic_pointer_cast<const ast::IfNode>(m_initializer)) {
                    return nullptr;
                }
                else {
                if (auto initializer_type = m_initializer->compute_stmt_type(st)) {
                if (m_type) {
                    if (auto type_name = std::dynamic_pointer_cast<const ast::Identifier>(m_type)) {
                        if (initializer_type->as_string() != type_name->as_string()) {
                            return set_error(fmt::format("Initializer type '{}' doesn't match explicit type '{}'", 
                            initializer_type->as_string(), type_name->as_string()));
                        }
                    }
                }
            }
        }
    } 

        return shared_from_this(); 
    }

private:
    Node::Ptr m_name;          
    Node::Ptr m_type;          
    Node::Ptr m_initializer;   
};

}

#endif // KIRAZ_AST_LET_NODE_H
