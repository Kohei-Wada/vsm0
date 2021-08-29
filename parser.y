%{
#include <stdio.h>
#include "instr.h"
#include "parse.h"

static parser_t *yyp = NULL;

extern int yyparse(void);
extern int yylex(void);
void yyerror(char *);

extern void yy_set_parser(parser_t *p);

%}


%union {
	int int_value;
	char *name;
};

%token <int_value> NUM ADDOP SUBOP MULOP DIVOP OROP ANDOP
%token <name> ID

%left ADDOP SUBOP
%left MULOP DIVOP MODOP ANDOP OROP


%%

program 
:expr_list  
{ 
	parser_handle_simple_op(yyp, HALT);
	YYACCEPT; 
}      
;


expr_list 
: 

| expr_list expr  ';'   
{ 
	parser_handle_simple_op(yyp, OUTPUT);
}

| expr_list error ';'  
{
	yyerrok;
}
;


expr

: expr ADDOP expr       
{ 
	parser_handle_simple_op(yyp, ADD);
}

| expr SUBOP expr       
{ 
	parser_handle_simple_op(yyp, SUB);
}

| expr MULOP expr       
{ 
	parser_handle_simple_op(yyp, MUL);
}

| expr DIVOP expr       
{ 
	parser_handle_simple_op(yyp, DIV);
}

| expr MODOP expr       
{ 
	parser_handle_simple_op(yyp, MOD);
}

| expr OROP expr       
{ 
	parser_handle_simple_op(yyp, OR);
}

| expr ANDOP expr       
{ 
	parser_handle_simple_op(yyp, AND);
}

| '(' expr ')'          
{ 

}

| NUM                   
{ 
	parser_handle_num(yyp, $1);
}
;

%%

void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

void yy_set_parser(parser_t *p)
{
	yyp = p;
}
