#ifndef KIRAZ_TOKEN_FUNCTOKENS_H
#define KIRAZ_TOKEN_FUNCTOKENS_H

#include <kiraz/Token.h>

namespace token {

class ArgNode : public Token {
public:
    ArgNode(std::shared_ptr<Token> name, std::shared_ptr<Token> type)
        : Token(IDENTIFIER), m_name(name), m_type(type) {}
    
    std::string as_string() const override {
        return fmt::format("ArgNode(name={}, type={})", m_name->as_string(), m_type->as_string());
    }

private:
    std::shared_ptr<Token> m_name;
    std::shared_ptr<Token> m_type;
};

class FuncArgs : public Token {
public:
    FuncArgs() : Token(OP_LPAREN) {}
    
    void add_argument(std::shared_ptr<Token> arg) {
        m_args.push_back(arg);
    }

    std::string as_string() const override {
        std::string result = "FuncArgs(";
        for (const auto& arg : m_args) {
            result += arg->as_string() + ", ";
        }
        result += ")";
        return result;
    }

private:
    std::vector<std::shared_ptr<Token>> m_args;
};

} // namespace token

#endif // KIRAZ_TOKEN_FUNCTOKENS_H
