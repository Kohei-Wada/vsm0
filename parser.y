%{
#include <stdio.h>
#include "vsm.h"
#include "instr.h"

static int yypc = 0;
static vsm_t *yyvsm = NULL;

extern int yyparse(void);
extern int yylex(void);
void yyerror(char *);

void yypc_inc(void);
void yypc_set(int pc);
extern void set_yyvsm(vsm_t *v);

%}

%token NUM
%left ADDOP SUBOP
%left MULOP DIVOP MODOP

%%

program 
:expr_list  
{ 
	vsm_set_instr(yyvsm, yypc, HALT, 0, 0); 
	yypc_set(0);
	YYACCEPT; 
}      
;


expr_list 
: 

| expr_list expr  ';'   
{ 
	vsm_set_instr(yyvsm, yypc, OUTPUT, 0, 0); 
	yypc_inc();
}


| expr_list error ';'  
{
	yyerrok;
}

;

expr

: expr ADDOP expr       
{ 
	vsm_set_instr(yyvsm, yypc, ADD, 0, 0); 
	yypc_inc();
}


| expr SUBOP expr       
{ 
	vsm_set_instr(yyvsm, yypc, SUB, 0, 0); 
	yypc_inc();
}


| expr MULOP expr       
{ 
	vsm_set_instr(yyvsm, yypc, MUL, 0, 0); 
	yypc_inc();
}


| expr DIVOP expr       
{ 
	vsm_set_instr(yyvsm, yypc, DIV, 0, 0); 
	yypc_inc();
}


| expr MODOP expr       
{ 
	vsm_set_instr(yyvsm, yypc, MOD, 0, 0); 
	yypc_inc();
}


| '(' expr ')'          
{ 

}


| NUM                   
{ 
	vsm_set_instr(yyvsm, yypc, PUSHI, 0, $1); 
	yypc_inc();
}

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


void yypc_set(int pc)
{
	yypc = pc;
}


void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

