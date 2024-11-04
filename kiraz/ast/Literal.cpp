
#include "Literal.h"

#include <cassert>
#include <kiraz/token/Literal.h>

namespace ast {
    Integer::Integer(Token::Ptr t) : Node(L_INTEGER){
        assert(t->get_id() == L_INTEGER);
        auto token_int = std::static_pointer_cast<const token::Integer>(t);
        auto base = token_int->get_base();

        try{
            m_value = std::stoll(token_int->get_value(), nullptr, base);
        }
        catch (const std::exception& e){
            //TODO Mark this node as invalid
        }
    }

    Identifier::Identifier(Token::Ptr t) : Node(IDENTIFIER) {
        assert(t->get_id() == IDENTIFIER);
        auto token_id = std::static_pointer_cast<const token::Identifier>(t);
        m_name = token_id->get_name();
    }

    StringLiteral::StringLiteral(Token::Ptr token) : Node(L_STRING) {
        assert(token->get_id() == L_STRING); 
        auto token_str = std::static_pointer_cast<const token::StringLiteral>(token);
        m_value = token_str->get_value();
    }

}
