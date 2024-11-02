%{
#include "lexer.hpp"

#include <kiraz/ast/Operator.h>
#include <kiraz/ast/Literal.h>
#include <kiraz/ast/LetNode.h>
#include <kiraz/ast/FuncNode.h>
#include <kiraz/token/keyword.h>
#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/FuncNode.h>

int yyerror(const char *msg);
extern std::shared_ptr<Token> curtoken;
extern int yylineno;
%}

%token    REJECTED

%token OP_LPAREN
%token OP_RPAREN

%token OP_PLUS
%token OP_MINUS
%token OP_MULT
%token OP_DIVF

%token L_INTEGER

%left OP_PLUS OP_MINUS
%left OP_MULT OP_DIVF

%token KW_FUNC  
%token KW_LET    
%token IDENTIFIER
%token OP_COLON
%token OP_LBRACE
%token OP_RBRACE
%token OP_COMMA
%token OP_SCOLON
%token OP_ASSIGN
%%

stmt:
    OP_LPAREN stmt OP_RPAREN { $$ = $2; }
    | addsub
    | let_stmt
    | func_stmt
    | stmt OP_SCOLON { $$ = $1; }
    ;


func_stmt:
    KW_FUNC type OP_LPAREN arg_list OP_RPAREN OP_COLON type OP_LBRACE stmt_list OP_RBRACE OP_SCOLON {
        $$ = Node::add<ast::FuncNode>($2, $4, $7, $9);
    }
    ;

arg_list:
    | {
        $$ = Node::add<ast::FuncArgs>();
    }
    | IDENTIFIER OP_COLON type {
        auto args = Node::add<ast::FuncArgs>();
        args->add_argument(Node::add<ast::ArgNode>(Node::add<ast::Identifier>(curtoken), $3));
        $$ = args;
    }
    | arg_list OP_COMMA IDENTIFIER OP_COLON type {
        auto args = std::dynamic_pointer_cast<ast::FuncArgs>($1);
        if (args) {
            args->add_argument(Node::add<ast::ArgNode>(Node::add<ast::Identifier>(curtoken), $5));
        }
        $$ = $1;
    }
    ;

stmt_list:
    | {
        $$ = Node::add<ast::NodeList>();
    }
    | stmt {
        auto stmts = Node::add<ast::NodeList>();
        stmts->add_node($1);
        $$ = stmts;
    }
    | stmt_list stmt {
        auto stmts = std::dynamic_pointer_cast<ast::NodeList>($1);
        if (stmts) {
            stmts->add_node($2);
        }
        $$ = $1;
    }
    ;


let_stmt:
    KW_LET type OP_ASSIGN expr OP_SCOLON { 
        $$ = Node::add<ast::LetNode>($2, nullptr, $4); 
    }
    | KW_LET type OP_COLON type OP_SCOLON { 
        $$ = Node::add<ast::LetNode>($2, $4, nullptr); 
    }
    | KW_LET type OP_COLON type OP_ASSIGN expr OP_SCOLON { 
        $$ = Node::add<ast::LetNode>($2, $4, $6); 
    }
    ;

type:
    IDENTIFIER { $$ = Node::add<ast::Identifier>(curtoken); }
    ;

expr:
    addsub
    | posneg
    ;


addsub:
    muldiv
    | stmt OP_PLUS stmt { $$ = Node::add<ast::OpAdd>($1, $3); }
    | stmt OP_MINUS stmt { $$ = Node::add<ast::OpSub>($1, $3); }
    ;

muldiv:
    posneg
    | stmt OP_MULT stmt { $$ = Node::add<ast::OpMult>($1, $3); }
    | stmt OP_DIVF stmt { $$ = Node::add<ast::OpDivF>($1, $3); }
    ;

posneg:
    type
    | L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); }
    | OP_PLUS stmt { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2); }
    | OP_MINUS stmt { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2); }
    ;

stmt: %empty

%%

int yyerror(const char *s) {
    if (curtoken) {
        fmt::print("** Parser Error at {}:{} at token: {}\n",
            yylineno, Token::colno, curtoken->as_string());
    }
    else {
        fmt::print("** Parser Error at {}:{}, null token\n",
            yylineno, Token::colno);
    }
    Token::colno = 0;
    Node::reset_root();
    return 1;
}
