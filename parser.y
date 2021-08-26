%{
#include <stdio.h>
#include "vsm.h"
#include "instr.h"

static int yypc = 0;
static vsm_t *yyvsm = NULL;
%}

%token NUM
%left ADDOP SUBOP
%left MULOP DIVOP MODOP

%%

program 
:expr_list  { vsm_set_instr(yyvsm, yypc, HALT, 0, 0); YYACCEPT; yypc_inc();}      
;

expr_list 
: 
| expr_list expr  ';'   { vsm_set_instr(yyvsm, yypc, OUTPUT, 0, 0); yypc_inc();}
| expr_list error ';'   { yyerrok;}
;

expr
: expr ADDOP expr       { $$ = $1 + $3; vsm_set_instr(yyvsm, yypc, ADD, 0, 0); yypc_inc();}
| expr SUBOP expr       { $$ = $1 - $3;}
| expr MULOP expr       { $$ = $1 * $3;}
| expr DIVOP expr       { $$ = $1 / $3;}
| expr MODOP expr       { $$ = $1 % $3;}
| '(' expr ')'          
| NUM                   { vsm_set_instr(yyvsm, yypc, PUSHI, 0, $1); yypc_inc();}
;

%%


void set_yyvsm(vsm_t *v)
{
	yyvsm = v;
}

void yypc_inc(void)
{
	++yypc;
}

void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

