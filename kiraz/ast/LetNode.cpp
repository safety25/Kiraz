#include "LetNode.h"
#include <cassert>
#include <kiraz/token/Literal.h>
#include <kiraz/token/keyword.h>

namespace ast {

LetNode::LetNode(Node::Ptr name, Node::Ptr type, Node::Ptr initializer)
    : Node(KW_LET), m_name(name), m_type(type), m_initializer(initializer) {

    assert(m_name != nullptr && m_name->get_id() == IDENTIFIER && "name must be an Identifier");

    if (m_type) {
        assert(m_type->get_id() == IDENTIFIER && "type must be a type identifier");
    }

    if (m_initializer) {
        int init_id = m_initializer->get_id();
        assert((init_id == L_INTEGER || init_id == L_SIGNED) && "initializer must be an Integer or a SignedNode");
    }
}


} // namespace ast
