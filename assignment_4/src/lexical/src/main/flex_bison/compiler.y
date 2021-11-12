/****************************************************/
/* File: cm.y                                       */
/* The C- Yacc/Bison specification file             */
/* Based on tiny.y source code from                 */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden, 1997                          */
/* Modified by Eom Taegyung                         */
/* Modified by Mariska IJpelaar                     */
/****************************************************/

/* C declarations */
%{
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <listtree.h>
#include <decllist.h>
#include <node.h>
#include <types.h>
#include "basevisitor.h"
#include "../cpp/debug/debug.h"

/* Import from comp.l*/
#ifdef __cplusplus
extern "C" {
#endif

int yylex();   /* Lexer function*/
int yywrap(void);
#ifdef __cplusplus
}
#endif

extern int lineno;        /* Current line number*/

#ifdef DEBUG
int msglevel = 0; /* higher = more debugging messages*/
#else
int msglevel = 0;   /* don't bother sensitive souls*/
#endif

static void yyerror(BaseVisitor&, const char*);
static std::string cvt_str(char* s);

%}

%code requires {
    class BaseVisitor;
    struct ListTree;
}

/* Start symbol*/
%start program

/* Tokens*/
/* reserved words */
%token ELSE IF INT INT8 UNSIGNED UINT8 RETURN VOID WHILE
/* special symbols */
%token PLUS MINUS TIMES OVER MOD AND OR LT LTE GT GTE EQ NEQ ASSIGN SEMI COMMA LPAREN RPAREN LBRACKET RBRACKET LBRACE RBRACE
/* multicharacter tokens */
%token ID NUM
/* endfile */
%token ENDFILE 0
/* unary symbols */
%token UMINUS NOT

/* TODO: uncomment once required */
/* Precedence directive to resolve
    shift-reduce conflict of dangling else problem. */
/* %right NOELSE ELSE */

/* Provides more useful error messages */
%define parse.error verbose

/* Types to pass between lexer, rules and actions*/
%union {
    char* idStr;
    char* numStr;
    Node* node;
    ListTree* listTree;
    ReturnType rt;
    NodeType nt;
}

%parse-param {BaseVisitor& vis}

%%

 /* GRAMMAR RULES AND ACTIONS*/
program             : 	{ vis.program_start(); }
			         declaration_list
                        { vis.visit_program(); }
                    ;
identifier          : ID
                        { $<idStr>$ = $<idStr>1; }
                    ;
number              : NUM
                        { $<numStr>$ = $<numStr>1; }
                    ;
declaration_list    : declaration_list declaration
                    | declaration
                    ;
declaration         : var_declaration
                    | fun_declaration
		            ;
var_declaration     : type_specifier var_identifier_list SEMI
                        { vis.register_declarations($<rt>1); }
                    ;
var_identifier_list : var_identifier_list COMMA var_identifier
        		    | var_identifier
        		    ;
var_identifier      : identifier
 			        { vis.visit_var_decl(cvt_str($<idStr>1)); }
type_specifier      : VOID
                        { $<rt>$ = RT_VOID; }
                    ;
fun_declaration     : type_specifier identifier LPAREN
                        { vis.visit_func_start($<rt>1, cvt_str(strdup($<idStr>2))); }
                        params RPAREN compound_stmt
                        { vis.visit_func_end(cvt_str($<idStr>2), $<node>7); }
                    ;
params              : VOID;
var_decl_list       : var_decl_list var_declaration | %empty;
compound_stmt       : LBRACE
        			{ vis.add_local_scope(); }
        			compound_body RBRACE
                        {
                          $<node>$ = $<node>3;
                          vis.leave_current_scope();
                        }
                    ;
compound_body       : var_decl_list compound_list
			{
			  ListTree* listTree = $<listTree>2;
			  if (listTree != nullptr) {
				$<node>$ = $<listTree>2->root;
				delete $<listTree>2;
			  } else {
				$<node>$ = vis.empty();
			  }
			}
		    | %empty
		    	{ $<node>$ = vis.empty(); }

compound_list       : compound_list statement
			{
			  ListTree* listTree = $<listTree>1;
			  if (listTree == nullptr)
				listTree = new ListTree();
			  vis.visit_statement_list($<node>2, *listTree);
			  $<listTree>$ = listTree;
			}
		    | statement
			{
			  ListTree* listTree = new ListTree();
			  vis.visit_statement_list($<node>1, *listTree);
			  $<listTree>$ = listTree;
			}
            ;
statement           : expression_stmt
			             { $<node>$ = $<node>1; }
                    | compound_stmt
                    	{ $<node>$ = $<node>1; }
                    | return_stmt
                    	{ $<node>$ = $<node>1; }
                    ;
expression_stmt     : expression SEMI
			         { $<node>$ = $<node>1; }
                    | SEMI
                    	{ $<node>$ = vis.empty(); }
                    ;
return_stmt         : RETURN expression SEMI
                        { $<node>$ = vis.visit_return($<node>2); }
                    | RETURN SEMI
                    	{ $<node>$ = vis.visit_return(vis.empty()); }
                    ;
expression          : var ASSIGN expression
                        { $<node>$ = vis.visit_assignment($<node>1, $<node>3); }
                    | bool_expression
                    	{ $<node>$ = $<node>1; }
                    ;
var                 : identifier
                        { $<node>$ = vis.visit_lvariable(cvt_str($<idStr>1)); }
                    ;
bool_expression     : bool_expression boolop simple_expression
        			{ $<node>$ = vis.visit_operator($<nt>2, $<node>1, $<node>3); }
        		    | simple_expression
        		    	{ $<node>$ = $<node>1;}
        		    ;
simple_expression   : simple_expression relop additive_expression
                        { $<node>$ = vis.visit_operator($<nt>2, $<node>1, $<node>3); }
                    | additive_expression
                    	{ $<node>$ = $<node>1; }
                    ;
boolop 		       : OR
                    	{ $<nt>$ = NODE_OR; }
                    | AND
                    	{ $<nt>$ = NODE_AND; }
relop               : EQ
                        { $<nt>$ = NODE_REL_EQUAL; }
                    | NEQ
                        { $<nt>$ = NODE_REL_NOTEQUAL; }
                    ;
additive_expression : additive_expression addop term
                        { $<node>$ = vis.visit_operator($<nt>2, $<node>1, $<node>3); }
                    | term
                    	{ $<node>$ = $<node>1; }
                    ;
addop               : PLUS
                        { $<nt>$ = NODE_ADD; }
                    ;
term                : term mulop factor
                        { $<node>$ = vis.visit_operator($<nt>2, $<node>1, $<node>3); }
                    | factor
                    	{ $<node>$ = $<node>1; }
                    ;
mulop               : TIMES
                        { $<nt>$ = NODE_MUL; }
                    ;
factor              : LPAREN expression RPAREN
			         { $<node>$ = $<node>2; }
                    | identifier
                    	{ $<node>$ = vis.visit_rvariable(cvt_str($<idStr>1)); }
                    | identifier LPAREN args RPAREN
                    	{ $<node>$ = vis.visit_funccall(cvt_str($<idStr>1), $<node>3); }
                    | number
                    	{ $<node>$ = vis.visit_number(cvt_str($<numStr>1)); }
                    | unary factor
                    	{ $<node>$ = vis.visit_operator($<nt>1, $<node>2); }
                    ;
args                : arg_list
			         { $<node>$ = $<listTree>1->root; delete $<listTree>1; }
                    | %empty
                    	{ $<node>$ = vis.empty(); }
                    ;
arg_list            : arg_list COMMA expression
        			{
        			  vis.visit_exprlist($<node>3, *$<listTree>1);
        			  $<listTree>$ =  $<listTree>1;
        			}
                    | expression
                    	{
                    	  ListTree* listTree = new ListTree();
                    	  vis.visit_exprlist($<node>1, *listTree);
                    	  $<listTree>$ = listTree;
                    	}
                    ;
unary		         : PLUS
			         { $<nt>$ = NODE_SIGNPLUS; }
		              ;
%%

static void yyerror(BaseVisitor& vis, const char* s) {
    (&vis)->logger.error(lineno) << s << '\n';
}

static std::string cvt_str(char* s) {
    std::string str(s);
    free(s);
    return str;
}

int yywrap() {
    return 1;
}
