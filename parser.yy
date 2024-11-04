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
#include <kiraz/ast/testModule.h>

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


%token OP_EQ
%token OP_GT
%token OP_GE
%token OP_LT
%token OP_LE

%start module


%%

stmt:
    OP_LPAREN stmt OP_RPAREN { $$ = $2; }
    | let_stmt
    | func_stmt
    | assign_stmt OP_SCOLON
    | expr OP_SCOLON
    ;

expr:
    OP_LPAREN expr OP_RPAREN { $$ = $2; }
    | addsub
    | posneg
    | expr OP_EQ expr { $$ = Node::add<ast::OpEq>($1, $3); }
    | expr OP_GT expr { $$ = Node::add<ast::OpGt>($1, $3); }
    | expr OP_GE expr { $$ = Node::add<ast::OpGe>($1, $3); }
    | expr OP_LT expr { $$ = Node::add<ast::OpLt>($1, $3); }
    | expr OP_LE expr { $$ = Node::add<ast::OpLe>($1, $3); }
    | type
    ;

module:
    | { 
        Node::reset_root(); 
        $$ = nullptr; 
    }
    | stmt { 
        $$ = Node::add<ast::Module>($1); 
    }
;

assign_stmt: 
    type OP_ASSIGN expr { $$ = Node::add<ast::AssignNode>($1, $3); }

func_stmt:
    KW_FUNC type OP_LPAREN arg_list OP_RPAREN OP_COLON type OP_LBRACE stmt_list OP_RBRACE OP_SCOLON {
        $$ = Node::add<ast::FuncNode>($2, $4, $7, $9);
    }
    ;

arg_list:
    | {
        $$ = Node::add<ast::FuncArgs>();
    }
    | type OP_COLON type {
        auto args = Node::add<ast::FuncArgs>();
        args->add_argument(Node::add<ast::ArgNode>($1, $3));  
        $$ = args;
    }
    | arg_list OP_COMMA type OP_COLON type {
        auto args = std::dynamic_pointer_cast<ast::FuncArgs>($1);
        if (args) {
            args->add_argument(Node::add<ast::ArgNode>($3, $5)); 
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

addsub:
    muldiv
    | expr OP_PLUS expr { $$ = Node::add<ast::OpAdd>($1, $3); }
    | expr OP_MINUS expr { $$ = Node::add<ast::OpSub>($1, $3); }
    ;

muldiv:
    posneg
    | expr OP_MULT expr { $$ = Node::add<ast::OpMult>($1, $3); }
    | expr OP_DIVF expr { $$ = Node::add<ast::OpDivF>($1, $3); }
    ;

posneg:
    type
    | L_INTEGER { $$ = Node::add<ast::Integer>(curtoken); }
    | OP_PLUS expr { $$ = Node::add<ast::SignedNode>(OP_PLUS, $2); }
    | OP_MINUS expr { $$ = Node::add<ast::SignedNode>(OP_MINUS, $2); }
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
