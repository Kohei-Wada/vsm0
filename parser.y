%{
#include <stdio.h>
#include <string.h>
#include "instr.h"
#include "parse.h"
#include "instr.h"
#include "jmpchain.h"
#include "label.h"

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
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, HALT, 0, 0); 
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
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, REMOVE, 0, 0); 
	parser_inc_pc(yyp);
}

| lbl_stmnt

| ';'

| '{' s_list '}'

| write_stmnt ';'

| read_stmnt ';'

| if_part 
{
	int pc = parser_get_pc(yyp);
	parser_back_patching(yyp, $1, pc);
}

| if_part ELSE
{
	int pc = parser_get_pc(yyp);
	$<int_value>$ = pc;
	parser_set_instr(yyp, pc, JUMP, 0, -1); 
	parser_inc_pc(yyp);
	parser_back_patching(yyp, $1, pc + 1);
}

  stmnt 
{
	parser_back_patching(yyp, $<int_value>3, parser_get_pc(yyp));
}


| FOR '(' opt_expr ';' 
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_nestin(j, FOR);
}

  tst_expr ';' 
{
	int pc = parser_get_pc(yyp);
	jmpchain_t *j = parser_get_jchain(yyp);
	parser_set_instr(yyp, pc, BNE, 0, -1); 
	parser_inc_pc(yyp);
	jmpchain_break(j, JUMP);
}

  opt_expr ')'
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, JUMP, 0, $3); 
	parser_inc_pc(yyp);
	parser_back_patching(yyp, $6, pc + 1);
}

  stmnt
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_conti(j);
	jmpchain_nestout(j, $6+2);
}

| WHILE 
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_nestin(j, FOR);
	$<int_value>$ = parser_get_pc(yyp);
}

  '(' expr ')'
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_break(j, BEQ);
}

  stmnt
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_conti(j);
	jmpchain_nestout(j, $<int_value>2);
}


| DO
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_nestin(j, WHILE);
	$<int_value>$ = parser_get_pc(yyp);
}

  stmnt 
{
	$<int_value>$ = parser_get_pc(yyp);
}

  WHILE  '(' expr ')'
{
	parser_set_instr(yyp, parser_get_pc(yyp), BNE, 0, $<int_value>2); 
	parser_inc_pc(yyp);
}

  ';'

{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_nestout(j, $<int_value>4);
}


| BREAK ';'
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_break(j, JUMP);
}

| CONTI ';'
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_conti(j);
}


| error ';'
{
	yyerrok;
}


| SWITCH '(' expr ')'
{
	int pc = parser_get_pc(yyp);
	jmpchain_t *j = parser_get_jchain(yyp);

	$<int_value>$ = pc;
	parser_set_instr(yyp, pc, JUMP, 0, -1);
	parser_inc_pc(yyp);

	jmpchain_nestin(j, 0);
	begin_switch();
}

  stmnt
{
	jmpchain_t *j = parser_get_jchain(yyp);
	jmpchain_break(j, JUMP);
	parser_back_patching(yyp, $<int_value>5, parser_get_pc(yyp));
	end_switch(yyp);
	jmpchain_nestout(j, 0);
}
;


write_stmnt
: WRITE expr 
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, OUTPUT, 0, 0); 
	parser_inc_pc(yyp);
}

| write_stmnt ',' expr 
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, OUTPUT, 0, 0); 
	parser_inc_pc(yyp);
}


read_stmnt
: READ LHS 
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, INPUT, 0, 0); 
	parser_inc_pc(yyp);
}

| read_stmnt ',' LHS
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, INPUT, 0, 0); 
	parser_inc_pc(yyp);
}


expr
: LHS '=' expr
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, ASSGN, 0, 0); 
	parser_inc_pc(yyp);
}

| expr '?' 
{
	int pc = parser_get_pc(yyp);
	$<int_value>$ = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, BEQ, 0, -1);
	parser_inc_pc(yyp);
}

  expr ':'
{
	int pc = parser_get_pc(yyp);
	$<int_value>$ = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, JUMP, 0, -1);
	parser_inc_pc(yyp);
	parser_back_patching(yyp, $<int_value>3, parser_get_pc(yyp));
}
  expr
{
	parser_back_patching(yyp, $<int_value>6, parser_get_pc(yyp));
}


| expr ADDOP expr       
{ 
	parser_set_instr(yyp, parser_get_pc(yyp), $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr MULOP expr       
{ 
	parser_set_instr(yyp, parser_get_pc(yyp), $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr LAND expr       
{ 
	parser_set_instr(yyp, parser_get_pc(yyp), $2, 0, 0); 
	parser_inc_pc(yyp);
}

| expr LOR expr       
{ 
	parser_set_instr(yyp, parser_get_pc(yyp), $2, 0, 0); 
	parser_inc_pc(yyp);
}


| expr RELOP expr       
{ 
	int pc = parser_get_pc(yyp);

	parser_set_instr(yyp, pc, COMP, 0, 0); 
	parser_set_instr(yyp, pc + 1, $2, 0, pc + 4);  
	parser_set_instr(yyp, pc + 2, PUSHI, 0, 0); 
	parser_set_instr(yyp, pc + 3, JUMP, 0, pc + 5); 
	parser_set_instr(yyp, pc + 4, PUSHI, 0, 1); 
	parser_set_pc(yyp, pc + 5);
}

| PPMM ID
{
	int addr = parser_sym_ref(yyp, $2);
	int pc = parser_get_pc(yyp);

	parser_set_instr(yyp, pc, PUSH, 0, addr); 
	parser_set_instr(yyp, pc + 1, $1, 0, 0); 
	parser_set_instr(yyp, pc + 2, COPY, 0, 0); 
	parser_set_instr(yyp, pc + 3, POP, 0, addr); 
	parser_set_pc(yyp, pc + 4);
}

| ID PPMM
{
	int addr = parser_sym_ref(yyp, $1);
	int pc = parser_get_pc(yyp);

	parser_set_instr(yyp, pc, PUSH, 0, addr); 
	parser_set_instr(yyp, pc + 1, COPY, 0, 0); 
	parser_set_instr(yyp, pc + 2, $2, 0, 0); 
	parser_set_instr(yyp, pc + 3, POP, 0, addr); 
	parser_set_pc(yyp, pc + 4);

}

| ADDOP expr %prec UM
{
	if ($1 == SUB) {
		int pc = parser_get_pc(yyp);
		parser_set_instr(yyp, pc, CSIGN, 0, 0); 
		parser_inc_pc(yyp);
	}
}

| '!' expr
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, NOT, 0, 0); 
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
	parser_set_instr(yyp, parser_get_pc(yyp), PUSHI, 0, $1); 
	parser_inc_pc(yyp);
}
;



if_part 
: IF '(' expr ')'  
{
	int pc = parser_get_pc(yyp);

	$<int_value>$ = pc;
	parser_set_instr(yyp, pc, BEQ, 0, -1);
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
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, REMOVE, 0, 0);
	parser_inc_pc(yyp);
	$$ = pc + 1;
}
;


tst_expr
: 
{
	int pc = parser_get_pc(yyp);
	parser_set_instr(yyp, pc, REMOVE, 0, 0);
	parser_inc_pc(yyp);
	$$ = pc + 1;
}

| expr
{
	$$ = parser_get_pc(yyp);
}


lbl_stmnt
: CASE NUM ':' 
{
	case_label($2, yyp);
}
  stmnt

| CASE ADDOP NUM ':'
{
	case_label($2 == SUB? -$3 : $3, yyp);
}
  stmnt

| DEFAULT ':'
{
	default_label(yyp);
}
  stmnt
;

	

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

