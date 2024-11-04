
%option noyywrap
%option yylineno

%{
// https://stackoverflow.com/questions/9611682/flexlexer-support-for-unicode/9617585#9617585
#include "main.h"
#include <kiraz/token/Literal.h>
#include <kiraz/token/Operator.h>
#include <kiraz/token/keyword.h>
static auto &colno = Token::colno;
using namespace token;
extern std::shared_ptr<Token> curtoken;
%}

%option yylineno

%%

[0-9]+ {colno += yyleng; curtoken = Token::New<Integer>(10, yytext); return L_INTEGER; }
"+" {colno += yyleng; curtoken = Token::New<OpPlus>(); return OP_PLUS; }
"-" {colno += yyleng; curtoken = Token::New<OpMinus>(); return OP_MINUS; }
"*" {colno += yyleng; curtoken = Token::New<OpMult>(); return OP_MULT; }
"/" {colno += yyleng; curtoken = Token::New<OpDivF>(); return OP_DIVF; }
"(" {colno += yyleng; curtoken = Token::New<OpLparen>(); return OP_LPAREN; }
")" {colno += yyleng; curtoken = Token::New<OpRparen>(); return OP_RPAREN; }

"func" { colno += yyleng; curtoken = Token::New<KwFunc>(); return KW_FUNC; }
"let" { colno += yyleng; curtoken = Token::New<KwLet>(); return KW_LET; }
"if"    { colno += yyleng; curtoken = Token::New<KwIf>(); return KW_IF; }
"else"  { colno += yyleng; curtoken = Token::New<KwElse>(); return KW_ELSE; }
"while" { colno += yyleng; curtoken = Token::New<KwWhile>(); return KW_WHILE; }
"import" { colno += yyleng; curtoken = Token::New<KwImport>(); return KW_IMPORT; }
"class"  { colno += yyleng; curtoken = Token::New<KwClass>(); return KW_CLASS; }

"," { colno += yyleng; curtoken = Token::New<OpComma>(); return OP_COMMA; }
";" { colno += yyleng; curtoken = Token::New<OpScolon>(); return OP_SCOLON; }
":" { colno += yyleng; curtoken = Token::New<OpColon>(); return OP_COLON; }
"{" { colno += yyleng; curtoken = Token::New<OpLbrace>(); return OP_LBRACE; }
"}" { colno += yyleng; curtoken = Token::New<OpRbrace>(); return OP_RBRACE; }
"=" { colno += yyleng; curtoken = Token::New<OpAssign>(); return OP_ASSIGN; }

"==" { colno += yyleng; curtoken = Token::New<OpEq>(); return OP_EQ; }
">"  { colno += yyleng; curtoken = Token::New<OpGt>(); return OP_GT; }
">=" { colno += yyleng; curtoken = Token::New<OpGe>(); return OP_GE; }
"<"  { colno += yyleng; curtoken = Token::New<OpLt>(); return OP_LT; }
"<=" { colno += yyleng; curtoken = Token::New<OpLe>(); return OP_LE; }

[a-zA-Z_][a-zA-Z0-9_]* {colno += yyleng; curtoken = Token::New<Identifier>(yytext); return IDENTIFIER; }


\"([^\"\\]|\\[\"\\n])*\" {
    colno += yyleng;
    std::string str_value(yytext + 1, yyleng - 2); 
    std::string processed_str;

    for (size_t i = 0; i < str_value.length(); ++i) {
        if (str_value[i] == '\\' && i + 1 < str_value.length()) {
            switch (str_value[i + 1]) {
                case 'n': processed_str += '\n'; ++i; break;
                case 't': processed_str += '\t'; ++i; break;
                case '\\': processed_str += '\\'; ++i; break;
                case '"': processed_str += '"'; ++i; break;
                default: break; 
            }
        } else {
            processed_str += str_value[i];
        }
    }
    curtoken = Token::New<token::StringLiteral>(processed_str);
    return L_STRING;
}


[ \n\t]+ {colno += yyleng;}
.       {colno += yyleng; curtoken = Token::New<Rejected>("reject"); return YYUNDEF; }

.        ;
 
