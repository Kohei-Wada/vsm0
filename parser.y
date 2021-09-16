%{
#include <stdio.h>
#include <string.h>
#include "instr.h"
#include "parse.h"
#include "vsm.h"
#include "jmpchain.h"

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

%token TYPE IF ELSE WHILE DO FOR
%token SWITCH CASE DEFAULT BREAK CONTI 
%token <int_value> NUM
%token <name> ID
%token <op> ADDOP MULOP LAND LOR WRITE READ PPMM RELOP

%right '='
%right '?' ':'
%left RELOP
%left ADDOP 
%left MULOP LAND LOR
%right '!' PPMM UM

%type <int_value> const_int expr if_part opt_expr tst_expr

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

| ';'

| '{' s_list '}'

| write_stmnt ';'

| read_stmnt ';'

| if_part 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_back_patching(v, $1, pc);
}

| if_part ELSE
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	$<int_value>$ = pc;

	vsm_set_instr(v, pc, JUMP, 0, -1); 
	parser_inc_pc(yyp);

	vsm_back_patching(v, $1, pc + 1);

}

stmnt 
{
	vsm_t *v = parser_get_vsm(yyp);
	vsm_back_patching(v, $<int_value>3, parser_get_pc(yyp));
}


| FOR '(' opt_expr ';' 
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_nestin(j, FOR);
}

  tst_expr ';' 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	jmpchain_t *j = parser_get_jchain(yyp);

	vsm_set_instr(v, pc, BNE, 0, -1); 
	parser_inc_pc(yyp);

	jmpchain_break(j, JUMP);
}

  opt_expr ')'
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);

	vsm_set_instr(v, pc, JUMP, 0, $3); 
	parser_inc_pc(yyp);

	vsm_back_patching(v, $6, pc + 1);
}


  stmnt
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_conti(j);
	jmpchain_nestout(j, $6+2);

}


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

| expr '?' 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);

	$<int_value>$ = parser_get_pc(yyp);
	vsm_set_instr(v, pc, BEQ, 0, -1);
	parser_inc_pc(yyp);
}

  expr ':'
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	$<int_value>$ = parser_get_pc(yyp);
	vsm_set_instr(v, pc, JUMP, 0, -1);
	parser_inc_pc(yyp);
	vsm_back_patching(v, $<int_value>3, parser_get_pc(yyp));
}
  expr
{
	vsm_t *v = parser_get_vsm(yyp);
	vsm_back_patching(v, $<int_value>6, parser_get_pc(yyp));
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
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);

	vsm_set_instr(v, pc, COMP, 0, 0); 
	vsm_set_instr(v, pc + 1, $2, 0, pc + 4);  
	vsm_set_instr(v, pc + 2, PUSHI, 0, 0); 
	vsm_set_instr(v, pc + 3, JUMP, 0, pc + 5); 
	vsm_set_instr(v, pc + 4, PUSHI, 0, 1); 

	parser_set_pc(yyp, pc + 5);
}

| PPMM ID
{
	vsm_t *v = parser_get_vsm(yyp);
	int addr = parser_sym_ref(yyp, $2);
	int pc = parser_get_pc(yyp);

	vsm_set_instr(v, pc, PUSH, 0, addr); 

	vsm_set_instr(v, pc + 1, $1, 0, 0); 
	vsm_set_instr(v, pc + 2, COPY, 0, 0); 
	vsm_set_instr(v, pc + 3, POP, 0, addr); 
	parser_set_pc(yyp, pc + 4);
}

| ID PPMM
{

	vsm_t *v = parser_get_vsm(yyp);
	int addr = parser_sym_ref(yyp, $1);
	int pc = parser_get_pc(yyp);

	vsm_set_instr(v, pc, PUSH, 0, addr); 

	vsm_set_instr(v, pc + 1, COPY, 0, 0); 
	vsm_set_instr(v, pc + 2, $2, 0, 0); 
	vsm_set_instr(v, pc + 3, POP, 0, addr); 
	parser_set_pc(yyp, pc + 4);

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
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);

	vsm_set_instr(v, pc, PUSHI, 0, $1); 
	parser_inc_pc(yyp);
}
;



if_part 
: IF '(' expr ')'  
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	$<int_value>$ = pc;
	vsm_set_instr(v, pc, BEQ, 0, -1);
	parser_inc_pc(yyp);
}

  stmnt
{
	$$ = $<int_value>5;
}


opt_expr
:
{
	$$ = parser_get_pc(yyp);
}
| expr
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, REMOVE, 0, 0);
	parser_inc_pc(yyp);
	$$ = pc + 1;
}
;


tst_expr
: 
{
	vsm_t *v = parser_get_vsm(yyp);
	int pc = parser_get_pc(yyp);
	vsm_set_instr(v, pc, REMOVE, 0, 0);
	parser_inc_pc(yyp);
	$$ = pc + 1;
}

| expr
{
	$$ = parser_get_pc(yyp);
}


LHS 
: ID
{  
	/*put the id address on the stack*/
	parser_handle_id(yyp, PUSHI, $1);
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


