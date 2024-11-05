#ifndef KIRAZ_AST_IFNODE_H
#define KIRAZ_AST_IFNODE_H

#include <kiraz/Node.h>
#include <string>

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

private:
    Node::Ptr m_condition;    
    Node::Ptr m_thenBranch;   
    Node::Ptr m_elseBranch;   
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

private:
    Node::Ptr m_condition;    
    Node::Ptr m_repeat;       
};

class ImportNode : public Node {
public:
    ImportNode(Node::Ptr name)
        : Node(KW_IMPORT), m_name(name) {}

    std::string as_string() const override {
        return fmt::format("Import({})", m_name ? m_name->as_string() : "null");
    }

private:
    Node::Ptr m_name; 
};

class ClassNode : public Node {
public:
    ClassNode(Node::Ptr name, Node::Ptr stmt_list)
        : Node(KW_CLASS), m_name(name), m_stmt_list(stmt_list) {}

    std::string as_string() const override {
        std::string name_str = m_name ? m_name->as_string() : "null";
        std::string stmt_list_str = m_stmt_list ? m_stmt_list->as_string() : "[]";
        return fmt::format("Class(n={}, s={})", name_str, stmt_list_str);
    }

private:
    Node::Ptr m_name;
    Node::Ptr m_stmt_list;
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

private:
    Node::Ptr m_value;
};


} 

#endif // KIRAZ_AST_IFNODE_H
