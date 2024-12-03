#ifndef KIRAZ_AST_OPERATOR_H
#define KIRAZ_AST_OPERATOR_H

#include <cassert>

#include <kiraz/Node.h>

namespace ast {
class OpBinary : public Node {
    protected:
        explicit OpBinary(int op, const Node::Ptr &left, const Node::Ptr &right)
            : Node(op), m_left(left), m_right(right){
                assert(left);
                assert(right);
            }

    public:
        auto get_left() const {return m_left; }
        auto get_right() const {return m_right; }

        std::string as_string() const override {
            assert(get_left());
            assert(get_right());

            std::string opstr;
                switch(get_id()) {
                    case OP_PLUS:
                        opstr = "Add";
                        break;
                    case OP_MINUS:
                        opstr = "Sub";
                        break;
                    case OP_MULT:
                        opstr = "Mult";
                        break;
                    case OP_DIVF:
                        opstr = "DivF";
                        break;
                    case OP_EQ: 
                        opstr = "OpEq"; 
                        break;
                    case OP_GT: 
                        opstr = "OpGt"; 
                        break;
                    case OP_GE: 
                        opstr = "OpGe"; 
                        break;
                    case OP_LT: 
                        opstr = "OpLt"; 
                        break;
                    case OP_LE: 
                        opstr = "OpLe"; 
                        break;
                    default:
                        break;
                }

                return fmt::format("{}(l={}, r={})", opstr, get_left()->as_string(), get_right()->as_string());
        }
        Node::Ptr check_type(SymbolTable &st) {
            auto left_type = m_left->compute_stmt_type(st);
            auto right_type = m_right->compute_stmt_type(st);

            if (get_id() == OP_PLUS) {
                if (left_type != right_type) {
                    return set_error(fmt::format("Operator '+' not defined for types '{}' and '{}'", 
                                                 left_type->as_string(), right_type->as_string()));
                }
            }

            return nullptr;
        }

private:
    Node::Ptr m_left, m_right;
};

class OpAdd : public OpBinary {
public:
    OpAdd(const Node::Ptr &left, const Node::Ptr & right) : OpBinary(OP_PLUS, left, right) {}
};

class OpSub : public OpBinary {
public:
    OpSub(const Node::Ptr &left, const Node::Ptr & right) : OpBinary(OP_MINUS, left, right) {}
};

class OpMult : public OpBinary {
public:
    OpMult(const Node::Ptr &left, const Node::Ptr & right) : OpBinary(OP_MULT, left, right) {}
};

class OpDivF : public OpBinary {
public:
    OpDivF(const Node::Ptr &left, const Node::Ptr & right) : OpBinary(OP_DIVF, left, right) {}
};

class AssignNode : public Node {
public:
    AssignNode(const Node::Ptr &left, const Node::Ptr &right)
        : Node(OP_ASSIGN), m_left(left), m_right(right) {
            assert(left);
            assert(right);
        }

    std::string as_string() const override {
        return fmt::format("Assign(l={}, r={})", m_left->as_string(), m_right->as_string());
    }

private:
    Node::Ptr m_left, m_right;
};

class OpEq : public OpBinary {
public:
    OpEq(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_EQ, left, right) {}
};

class OpGt : public OpBinary {
public:
    OpGt(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_GT, left, right) {}
};

class OpGe : public OpBinary {
public:
    OpGe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_GE, left, right) {}
};

class OpLt : public OpBinary {
public:
    OpLt(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_LT, left, right) {}
};

class OpLe : public OpBinary {
public:
    OpLe(const Node::Ptr &left, const Node::Ptr &right) : OpBinary(OP_LE, left, right) {}
};


}

#endif // KIRAZ_AST_OPERATOR_H
