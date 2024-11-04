#ifndef KIRAZ_AST_LITERAL_H
#define KIRAZ_AST_LITERAL_H

#include <kiraz/Node.h>

namespace ast {
class Integer : public Node {
public:
    Integer(Token::Ptr);

    std::string as_string() const override {return fmt::format("Int({})", m_value); }

private:
    int64_t m_value;
};

class SignedNode : public Node {
public:
    SignedNode(int op, Node::Cptr operand) : Node(L_INTEGER), m_operator(op), m_operand(operand) {}

    std::string as_string() const override {
        std::string op_str = (m_operator == OP_MINUS) ? "OP_MINUS" : "OP_PLUS";
        return fmt::format("Signed({}, {})", op_str, m_operand->as_string());
    }

private:
    int m_operator;
    Node::Cptr m_operand;
};

class Identifier : public Node {
public:
    Identifier(Token::Ptr token);

    std::string as_string() const override { return fmt::format("Id({})", m_name); }

private:
    std::string m_name;
};

class StringLiteral : public Node {
public:
    StringLiteral(Token::Ptr token);

    std::string as_string() const override { 
        return fmt::format("Str({})", m_value); 
    }

private:
    std::string m_value;
};


}

#endif
