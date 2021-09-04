%{
#include <stdio.h>
#include <string.h>
#include "instr.h"
#include "parse.h"

static parser_t *yyp = NULL;

extern int yyparse(void);
extern int yylex(void);
void yyerror(char *);

extern void yy_set_parser(parser_t *p);
extern void yy_set_yyin(FILE *f);
%}


%union {
	op_t op;
	int int_value;
	char *name;
};

%token TYPE
%token <int_value> NUM
%token <name> ID
%token <op> ADDOP MULOP LAND LOR WRITE READ PPMM RELOP

%right '='
%left RELOP
%left ADDOP 
%left MULOP LAND LOR
%right '!' PPMM UM

%type <int_value> const_int

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
	parser_sym_decl(yyp, $2, 0);
}
| decl ',' ID
{
	parser_sym_decl(yyp, $3, 0);
}

| TYPE ID '=' const_int
{
	parser_sym_decl(yyp, $2, $4);
}

| decl ',' ID '=' const_int
{
	parser_sym_decl(yyp, $3, $5);
}
;


const_int
: NUM
{
	$$ = $1;
}

| ADDOP NUM %prec UM
{
	$$ = $1 == SUB ? -$2 : $2;
}


LHS 
: ID
{  
	/*put the id address on the stack*/
	parser_handle_id(yyp, PUSHI, $1);
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

| write_stmnt ';'

| read_stmnt ';'


| error ';'
{
	yyerrok;
}


write_stmnt
: WRITE expr 
{
	parser_handle_simple_op(yyp, OUTPUT);
}

| write_stmnt ',' expr 
{
	parser_handle_simple_op(yyp, OUTPUT);
}


read_stmnt
: READ LHS 
{
	parser_handle_simple_op(yyp, INPUT);
}

| read_stmnt ',' LHS
{
	parser_handle_simple_op(yyp, INPUT);
}


expr
: LHS '=' expr
{
	parser_handle_simple_op(yyp, ASSGN);
}

| expr ADDOP expr       
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

| expr RELOP expr       
{ 
	parser_handle_relop(yyp, $2);
}

| PPMM ID
{
	parser_handle_ppmm(yyp, $1, $2, 1); 
}

| ADDOP expr %prec UM
{
	if ($1 == SUB)
		parser_handle_simple_op(yyp, CSIGN); 
}

| ID PPMM
{
	parser_handle_ppmm(yyp, $2, $1, 0); 
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
	/*put the id value on the stack*/
	parser_handle_id(yyp, PUSH, $1);
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


void yy_set_yyin(FILE *f)
{
	extern FILE *yyin;
	yyin = f;
}


