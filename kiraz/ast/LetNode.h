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
                return set_error(fmt::format("Variable '{}' is already defined", var_name->get_name()));
            }

            st.add_symbol(var_name->get_name(), shared_from_this());
        } else {
            return set_error("LetNode name must be an identifier");
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
            if (auto initializer_type = m_initializer->compute_stmt_type(st)) {
                if (m_type) {
                    if (auto expected_type = std::dynamic_pointer_cast<const ast::Identifier>(m_type)) {
                        if (expected_type->get_name() != initializer_type->as_string()) {
                            return set_error(fmt::format("Initializer type '{}' is not compatible with expected type '{}'",
                                                         initializer_type->as_string(), expected_type->get_name()));
                        }
                    }
                }
            }
        }

         if (auto while_node = std::dynamic_pointer_cast<const ast::WhileNode>(m_initializer)) {
                return set_error("While loops cannot be used as initializer expressions in LetNode");
            }
            if (auto if_node = std::dynamic_pointer_cast<const ast::IfNode>(m_initializer)) {
                return set_error("If expressions cannot be used as initializer expressions in LetNode");
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
