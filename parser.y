%{
#include <stdio.h>
#include <string.h>
#include "instr.h"
#include "parse.h"
#include "vsm.h"

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

%type <int_value> const_int expr

%%

/*TODO change program to declare variables at any time.*/

program 
: decl_list s_list
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, HALT, 0, 0); 
	parser_inc_pc(yyp);
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
	parser_sym_decl(yyp, $2,0);
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
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, REMOVE, 0, 0); 
	parser_inc_pc(yyp);
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
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, OUTPUT, 0, 0); 
	parser_inc_pc(yyp);
}

| write_stmnt ',' expr 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, OUTPUT, 0, 0); 
	parser_inc_pc(yyp);
}


read_stmnt
: READ LHS 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, INPUT, 0, 0); 
	parser_inc_pc(yyp);
}

| read_stmnt ',' LHS
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, INPUT, 0, 0); 
	parser_inc_pc(yyp);
}


expr
: LHS '=' expr
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, ASSGN, 0, 0); 
	parser_inc_pc(yyp);
}

| expr ADDOP expr       
{ 
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr MULOP expr       
{ 
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr LAND expr       
{ 
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr LOR expr       
{ 
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, $2, 0, 0); 
	parser_inc_pc(yyp);
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
	if ($1 == SUB) {
		vsm_t *v = parser_get_vsm(yyp);
		int pc = parser_get_pc(yyp);
		vsm_set_instr(v, pc, CSIGN, 0, 0); 
		parser_inc_pc(yyp);
	}
}

| ID PPMM
{
	parser_handle_ppmm(yyp, $2, $1, 0); 
}

| '!' expr
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, NOT, 0, 0); 
	parser_inc_pc(yyp);
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


