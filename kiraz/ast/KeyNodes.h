#ifndef KIRAZ_AST_IFNODE_H
#define KIRAZ_AST_IFNODE_H

#include <kiraz/Node.h>
#include <kiraz/Compiler.h>
#include <string>
#include "Literal.h"
#include <kiraz/ast/FuncNode.h>


namespace ast {

class IfNode : public Node {
public:
    IfNode(Node::Ptr condition, Node::Ptr thenBranch, Node::Ptr elseBranch)
        : Node(KW_IF), m_condition(condition), m_thenBranch(thenBranch), m_elseBranch(elseBranch) {}

    std::string as_string() const override {
        std::string result = fmt::format("If(?={}, then={}, else={})", 
                                          m_condition ? m_condition->as_string() : "null",
                                          m_thenBranch ? m_thenBranch->as_string() : "[]",
                                          m_elseBranch ? m_elseBranch->as_string() : "[]");
        return result;
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
    set_cur_symtab(st.get_cur_symtab());
    
    if (auto ret = m_condition->compute_stmt_type(st)) {
        return ret; 
    }

    if (auto condition = std::dynamic_pointer_cast<ast::Integer>(m_condition)) {
        return set_error("If only accepts tests of type 'Boolean'");
    }

    if (auto condition = std::dynamic_pointer_cast<ast::StringLiteral>(m_condition)) {
        return set_error("If only accepts tests of type 'Boolean'");
    }

    if (auto condition = std::dynamic_pointer_cast<ast::Identifier>(m_condition)) {
        return set_error("If only accepts tests of type 'Boolean'");
    }

    if (ScopeType::Module == st.get_scope_type() || ScopeType::Class == st.get_scope_type()) {
        return set_error("Misplaced if statement");
    }

    if (auto ret = m_thenBranch->compute_stmt_type(st)) {
        return ret; 
    }

    if (m_elseBranch) {
        if (auto ret = m_elseBranch->compute_stmt_type(st)) {
            return ret;
        }
    }

    return nullptr;
}


private:
    Node::Ptr m_condition;    
    Node::Ptr m_thenBranch;   
    Node::Ptr m_elseBranch;
    std::unique_ptr<SymbolTable> m_symtab;          
};

class WhileNode : public Node {
public:
    WhileNode(Node::Ptr condition, Node::Ptr repeat)
        : Node(KW_WHILE), m_condition(condition), m_repeat(repeat) {}

    std::string as_string() const override {
        return fmt::format("While(?={}, repeat={})", 
                           m_condition ? m_condition->as_string() : "null",
                           m_repeat ? m_repeat->as_string() : "[]");
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
    if (auto ret = m_condition->compute_stmt_type(st)) {
        return ret; 
    }

    if (ScopeType::Module == st.get_scope_type()){
        return set_error("Misplaced while statement");
    } 

    if (auto ret = m_repeat->compute_stmt_type(st)) {
        return ret;  
    }

    return nullptr; 
}

private:
    Node::Ptr m_condition;    
    Node::Ptr m_repeat;
    std::unique_ptr<SymbolTable> m_symtab;       
};

class ImportNode : public Node {
public:
    ImportNode(Node::Ptr name)
        : Node(KW_IMPORT), m_name(name) {}

    std::string as_string() const override {
        return fmt::format("Import({})", m_name ? m_name->as_string() : "null");
    }


    Node::Ptr compute_stmt_type(SymbolTable &st) override {
    if (!st.get_symbol(m_name->as_string())) {
        return set_error(fmt::format("Identifier '{}' is not found", m_name->as_string()));
    }
    return nullptr; 
    }
    

private:
    Node::Ptr m_name; 
};

class CallNode : public Node {
public:
    CallNode(Node::Ptr name, Node::Ptr args)
        : Node(-1), m_name(name), m_args(args) {}

    std::string as_string() const override {
        return fmt::format("Call(n={}, a={})", m_name->as_string(), m_args->as_string());
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
        set_cur_symtab(st.get_cur_symtab());

        if (auto ret = m_name->compute_stmt_type(st)) {
            return ret;
        }

        auto funcIdentifier = std::dynamic_pointer_cast<ast::Identifier>(m_name);
        if (!funcIdentifier) {
            return set_error("Function name is not a valid identifier.");
        }

        auto curScope = st.get_cur_symtab(); 
        
        auto funcSymbol = curScope->find(funcIdentifier->get_name());
        if (funcSymbol == curScope->end()) {
            return set_error(fmt::format("Identifier '{}' is not found", funcIdentifier->get_name()));
        }

        auto funcNode = std::dynamic_pointer_cast<ast::FuncNode>(funcSymbol->second);
        auto paramCount = funcNode->get_param_count();
        auto givenArgs = m_args->get_list(); 
        if (paramCount != givenArgs.size()) {
            return set_error(fmt::format("Call to function '{}' has wrong number of arguments", funcIdentifier->get_name()));
        }

        return nullptr;
    }

private:
    Node::Ptr m_name;  
    Node::Ptr m_args;
};

class ClassNode : public Node {
public:
    ClassNode(Node::Ptr name, Node::Ptr stmt_list, Node::Cptr parent = nullptr)
        : Node(KW_CLASS), m_name(name), m_stmt_list(stmt_list), m_parent(parent) {}

    std::string as_string() const override {
        std::string name_str = m_name ? m_name->as_string() : "null";
        std::string stmt_list_str = m_stmt_list ? m_stmt_list->as_string() : "[]";
        return fmt::format("Class(n={}, s={})", name_str, stmt_list_str);
    }

    Node::Ptr compute_stmt_type(SymbolTable &st) override {
        set_cur_symtab(st.get_cur_symtab());

        if (auto class_name = std::dynamic_pointer_cast<const ast::Identifier>(m_name)) {
            if (st.get_symbol(class_name->get_name())) {
                return set_error(fmt::format("Class '{}' is already defined", class_name->get_name()));
            }

            st.add_symbol(class_name->get_name(), shared_from_this());
        } else {
            return set_error("Class name must be an identifier");
        }

        if (m_parent) {
            if (auto parent_class_name = std::dynamic_pointer_cast<const ast::Identifier>(m_parent)) {
                if (!st.get_symbol(parent_class_name->get_name())) {
                    return set_error(fmt::format("Type '{}' is not found", parent_class_name->get_name()));
                }
            } else {
                return set_error("Class parent must be an identifier");
            }
        }

        if (m_stmt_list) {
            m_stmt_list->compute_stmt_type(st);
        }

            if (auto class_name = std::dynamic_pointer_cast<const ast::Identifier>(m_name)) {
            if (st.get_symbol(class_name->get_name())) {
                return set_error(fmt::format("Identifier '{}' is already in symtab", class_name->get_name()));
            }
        }

        

        return shared_from_this();
    }

private:
    Node::Ptr m_name;        
    Node::Ptr m_stmt_list;   
    Node::Cptr m_parent;    
    std::unique_ptr<SymbolTable> m_symtab;  
};



class Combined : public Node {
public:
    Combined() : Node(-2) {}

    void add_node(Node::Ptr node) {
        m_nodes.push_back(node);
    }

    std::string as_string() const override {
        std::string result = "";
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            result += m_nodes[i]->as_string();
            if (i < m_nodes.size() - 1) result += ", ";
        }
        result += "";
        return result;
    }

private:
    std::vector<Node::Ptr> m_nodes;  
};

class ReturnNode : public Node {
public:
    explicit ReturnNode(Node::Ptr value)
        : Node(KW_RETURN), m_value(value) {}

    std::string as_string() const override {
        return fmt::format("Return({})", m_value ? m_value->as_string() : "null");
    }

     Node::Ptr compute_stmt_type(SymbolTable &st) override {
        auto parent = dynamic_cast<ast::FuncNode*>(get_parent());
        if (!parent) {
            return set_error("Misplaced return statement");  
        }
        
        auto func_name = parent->get_name();  
        auto return_type = parent->get_return_type();  

        if (!return_type) {
            return set_error(fmt::format("Return type '{}' of function '{}' is not found", return_type->as_string(), func_name->as_string()));
        }

        if (m_value) {
            auto value_type = m_value->compute_stmt_type(st);
            if (value_type && value_type != return_type) {
                return set_error(fmt::format("Return statement type '{}' does not match function return type '{}'",
                                             value_type->as_string(), return_type->as_string()));
            }
        }

        return nullptr;  
    }


private:
    Node::Ptr m_value;
};

class DotNode : public Node {
public:
    DotNode(Node::Ptr left, Node::Ptr right)
        : Node(OP_DOT), m_left(left), m_right(right) {}

    std::string as_string() const override {
        return fmt::format("Dot(l={}, r={})", m_left->as_string(), m_right->as_string());
    }

private:
    Node::Ptr m_left, m_right;
};




} 

#endif // KIRAZ_AST_IFNODE_H
