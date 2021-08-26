%{
#include <stdio.h>
#include "vsm.h"
#include "instr.h"
%}

%token NUM
%left ADDOP SUBOP
%left MULOP DIVOP MODOP

%%

program   : expr_list '\n'       
		  ;

expr_list : 
		  | expr_list expr  ';'   { printf("%d\n", $2);}
		  | expr_list error ';'   { yyerrok;}
		  ;

expr      : expr ADDOP expr       { $$ = $1 + $3;}
	      | expr SUBOP expr       { $$ = $1 - $3;}
	      | expr MULOP expr       { $$ = $1 * $3;}
	      | expr DIVOP expr       { $$ = $1 / $3;}
	      | expr MODOP expr       { $$ = $1 % $3;}
	      | '(' expr ')'          { $$ = $2;}
	      | NUM                   { $$ = $1;}
	      ;

%%


void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

