#ifndef KIRAZ_TOKEN_KEYWORD_H
#define KIRAZ_TOKEN_KEYWORD_H

#include <kiraz/Token.h>

namespace token {

class Keyword : public Token {
public:
    Keyword(int type) : Token(type) {}
    virtual ~Keyword() {}
};


class KwLet : public Keyword {
public:
    KwLet() : Keyword(KW_LET) {}
    std::string as_string() const override { return "KW_LET"; }
};


class KwFunc : public Keyword {
public:
    KwFunc() : Keyword(KW_FUNC) {}
    std::string as_string() const override { return "KW_FUNC"; }
};

class KwIf : public Keyword {
public:
    KwIf() : Keyword(KW_IF) {}
    std::string as_string() const override { return "KW_IF"; }
};

class KwElse : public Keyword {
public:
    KwElse() : Keyword(KW_ELSE) {}
    std::string as_string() const override { return "KW_ELSE"; }
};

class KwWhile : public Keyword {
public:
    KwWhile() : Keyword(KW_WHILE) {}
    std::string as_string() const override { return "KW_WHILE"; }
};

class KwImport : public Keyword {
public:
    KwImport() : Keyword(KW_IMPORT) {}
    std::string as_string() const override { return "KW_IMPORT"; }
};

}

#endif // KIRAZ_TOKEN_KEYWORD_H
