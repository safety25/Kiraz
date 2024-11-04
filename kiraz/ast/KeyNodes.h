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
    Node::Ptr m_condition;    // Döngü koşulu
    Node::Ptr m_repeat;       // Döngü içinde tekrar eden ifadeler
};

class ImportNode : public Node {
public:
    ImportNode(Node::Ptr name)
        : Node(KW_IMPORT), m_name(name) {}

    std::string as_string() const override {
        return fmt::format("Import({})", m_name ? m_name->as_string() : "null");
    }

private:
    Node::Ptr m_name; // İçe aktarılan modül veya dosyanın adı
};


} 

#endif // KIRAZ_AST_IFNODE_H
