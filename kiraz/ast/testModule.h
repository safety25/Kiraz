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
        if (!m_symtab) { 
            m_symtab = std::make_shared<SymbolTable>();
        }
        
        if (m_root) {
            fmt::print("m_root type: {}\n", typeid(m_root).name());
        } else {
            fmt::print("m_root is null\n");
        }

        set_cur_symtab(st.get_cur_symtab());
        if(m_root){  
            auto node_list = std::dynamic_pointer_cast<ast::NodeList>(m_root);
            fmt::print("test{}",node_list==nullptr);
            if (node_list) {
                auto scope = st.enter_scope(ScopeType::Module, shared_from_this());
                for (const auto &stmt : node_list->get_list()){
                    if (!stmt){
                        return set_error("testerror");
                    }

                    if (auto ret = stmt ->add_to_symtab_forward(st)) {
                        return ret;
                    }
                }

                for (const auto &stmt : node_list->get_list()){
                    if (auto ret = stmt -> add_to_symtab_ordered(st)) {
                        return ret;

                    }
                    if (auto ret= stmt ->compute_stmt_type(st)){
                        return ret;
                    }
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