%option noyywrap noinput nounput
%option never-interactive
%{
/* lexeme of identifier or reserved word */
const int MAXTOKENLEN = 40;
char tokenString[MAXTOKENLEN+1];
int lineno = 1;
%}

%x COMMENT

/* C declarations */
%{
 
#include <stdio.h>
#include <string>
#include "../cpp/debug/debug.h"
#include "../public/node.h"
#include "../public/types.h"
#include "bison.compiler.h"

/* extern C declarations */
#if defined(__cplusplus)
extern "C" {
#endif

/* should be defined in stdio.h */
extern int fileno(FILE *);
int yylex(void);

#if defined(__cplusplus)
}
#endif
%}


digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  [a-zA-Z_][a-zA-Z0-9_]*
newline     \n
whitespace  [ \t]+

/* Patterns */
%%

"else"          {return ELSE;}
"if"            {return IF;}
"int"           {return INT;}
"int8_t"        {return INT8;}
"unsigned"      {return UNSIGNED;}
"uint8_t"       {return UINT8;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}

"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"%"             {return MOD;}
"<"             {return LT;}
"<="            {return LTE;}
">"             {return GT;}
">="            {return GTE;}
"=="            {return EQ;}
"!="            {return NEQ;}
"="             {return ASSIGN;}
"||"            {return OR;}
"&&"            {return AND;}
"!"             {return NOT;}
";"             {return SEMI;}
","             {return COMMA;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}


{number}        {
                    yylval.numStr = strdup(yytext);
                    return NUM;
                }
{identifier}    {
                    yylval.idStr = strdup(yytext);
                    return ID;
                }
{newline}       {++lineno;}
{whitespace}    {/* skip whitespace */}

"/*"            {BEGIN(COMMENT); }
<COMMENT>.      {/* eat up comment body */}
<COMMENT>{newline} {/* yylineno increments */}
<COMMENT><<EOF>> {
                    BEGIN(INITIAL);
                    pmesg(90, "[LEXER] Unclosed Command Found: %s\n", yytext);
                    return 1;
                }
<COMMENT>"*/"   {BEGIN(INITIAL); }

<<EOF>>         {return ENDFILE;}

.|\n            {
                    /* if no rule can be applied, give an error */
                    pmesg(90, "[LEXER] cannot match symbol: %s\n", yytext);
                    return 1;
                }

%%
