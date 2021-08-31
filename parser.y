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

%token TYPE
%token <int_value> NUM ADDOP MULOP LAND LOR WRITE
%token <name> ID

%right '='
%left ADDOP 
%left MULOP LAND LOR
%right '!'

%%


program 
: decl_list s_list
{
	parser_handle_simple_op(yyp, HALT);
	YYACCEPT; 
}
;


decl_list
:
| decl_list decl ';'
;


decl
: TYPE ID
{
	parser_sym_decl(yyp, $2);
}
| decl ',' ID
{
	parser_sym_decl(yyp, $3);
}
;


s_list 
: stmnt
| s_list stmnt
;


stmnt
: expr ';'
{
	parser_handle_simple_op(yyp, REMOVE);
}

| WRITE expr ';'
{
	parser_handle_simple_op(yyp, OUTPUT);
}

| error ';'
{
	yyerrok;
}


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

| ID                   
{ 
	parser_cout(yyp, PUSH, parser_sym_ref(yyp, $1));
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
