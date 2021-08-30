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
	op_t op;
	int int_value;
	char *name;
};

%token <int_value> NUM ADDOP MULOP LAND LOR 
%token <name> ID

%left ADDOP 
%left MULOP LAND LOR

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
	parser_handle_simple_op(yyp, $2);
}

| expr MULOP expr       
{ 
	parser_handle_simple_op(yyp, $2);
}

| expr LAND expr       
{ 
	parser_handle_simple_op(yyp, $2);
}

| expr LOR expr       
{ 
	parser_handle_simple_op(yyp, $2);
}


| '!' expr
{
	parser_handle_simple_op(yyp, NOT);
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
