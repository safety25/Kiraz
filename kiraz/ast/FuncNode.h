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
        : Node(IDENTIFIER), m_name(name), m_type(type) {}

    std::string as_string() const override {
        return fmt::format("FArg(n={}, t={})", 
                           m_name ? m_name->as_string() : "null",
                           m_type ? m_type->as_string() : "null");
    }

    std::string get_type() const override {
        return m_type ? m_type->as_string() : "";  
    }

private:
    Node::Ptr m_name;
    Node::Ptr m_type;
};


class FuncArgs : public Node {
public:
    FuncArgs() : Node(OP_LPAREN) {}
    
    std::vector<Node::Ptr> get_list() const override {
        return m_args;  
    }

    void add_argument(Node::Ptr arg) {
        m_args.push_back(arg);
    }

    size_t size() const {
        return m_args.size(); 
    }

    Node::Ptr get_argument(size_t index) const {
        if (index < m_args.size()) {
            return m_args[index];  
        }
        return nullptr;
    }
    

    std::string as_string() const override {
    if (m_args.empty()) {
        return "[]";
    }
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


class NodeList : public Node {
public:
    NodeList() : Node(OP_LBRACE) {}

    void add_node(Node::Ptr node) {
        m_nodes.push_back(node);
    }

    std::vector<Node::Ptr> get_list() const override {
        return m_nodes; 
    }

    std::string as_string() const override {
        std::string result = "[";
        for (size_t i = 0; i < m_nodes.size(); ++i) {
            result += m_nodes[i]->as_string();
            if (i < m_nodes.size() - 1) result += ", ";
        }
        result += "]";
        return result;
    }

private:
    std::vector<Node::Ptr> m_nodes;
};

class FuncNode : public Node {
public:
    FuncNode(Node::Ptr name, Node::Ptr args, Node::Ptr returnType, Node::Ptr body)
        : Node(KW_FUNC), m_name(name), m_args(args), 
          m_returnType(returnType), m_body(body) {}

    std::string as_string() const override {
        return fmt::format("Func(n={}, a={}, r={}, s={})",
                           m_name ? m_name->as_string() : "null", 
                           m_args ? m_args->as_string() : "[]", 
                           m_returnType ? m_returnType->as_string() : "null", 
                           m_body ? m_body->as_string() : "[]");
    }

    Node::Ptr get_return_type() const { 
        return m_returnType; 
    }

    Node::Ptr get_name() const { 
        return m_name; 
    }

    size_t get_param_count() const {
        if (auto args = std::dynamic_pointer_cast<FuncArgs>(m_args)) {
            return args->size(); 
        }
        return 0;
    }

    std::string get_param_type(size_t index) const {
        if (auto args = std::dynamic_pointer_cast<FuncArgs>(m_args)) {
            if (index < args->size()) {
                auto arg = args->get_argument(index);
                return arg->get_type(); 
            }
        }
        return "";  
    }

private:
    Node::Ptr m_name;        
    Node::Ptr m_args;         
    Node::Ptr m_returnType;   
    Node::Ptr m_body;         
};

}

#endif // KIRAZ_AST_FUNCNODE_H