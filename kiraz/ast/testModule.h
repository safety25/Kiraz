#ifndef KIRAZ_AST_MODULE_H
#define KIRAZ_AST_MODULE_H

#include <kiraz/Node.h>
#include <kiraz/Compiler.h>
#include <kiraz/ast/FuncNode.h>


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

    Node::Ptr compute_stmt_type(SymbolTable &st) override{
        if(m_root){
            assert(m_root -> is_stmt_list());
            auto scope = st.enter_scope(ScopeType::Module, shared_from_this());
            
            for (const auto &stmt : static_cast<const NodeList &> (*m_root).get_list()){
                if (auto ret = stmt ->add_to_symtab_forward(st)) {
                    return ret;
                }

                if (auto ret = stmt -> add_to_symtab_forward(*m_symtab)) {
                    return ret;

                    }
                }

            for (const auto &stmt :static_cast<const NodeList &>(*m_root).get_list()){
                if (auto ret = stmt -> add_to_symtab_ordered(st)) {
                    return ret;

                }

                if (auto ret= stmt -> add_to_symtab_ordered(*m_symtab)){
                    return ret;
                }

                if (auto ret= stmt ->compute_stmt_type(st)){
                    return ret;
                }
            }
        }
        return nullptr;
    }

private:
    Node::Ptr m_root;
    std::shared_ptr<SymbolTable> m_symtab;
};



} 

#endif // KIRAZ_AST_MODULE_H