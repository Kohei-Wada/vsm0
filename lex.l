%{
#include <stdio.h>
#include "y.tab.h"
#include "vsm.h"
#include "instr.h"
%}


letter [a-zA-Z_]

id     {letter}({letter}|[0-9])*

integer 0|[1-9][0-9]*
symbol [();]
other .


%%

{integer} { yylval.int_value = atoi(yytext); return NUM; }

{symbol}  { return yytext[0];}

"+"       { yylval.int_value = ADD; return ADDOP; }
"-"       { yylval.int_value = SUB; return SUBOP; }
"*"       { yylval.int_value = MUL; return MULOP; }
"/"       { yylval.int_value = DIV; return DIVOP; }
"%"       { yylval.int_value = MOD; return MODOP; }
"||"      { yylval.int_value = OR;  return OROP; }
"&&"      { yylval.int_value = AND; return ANDOP; }


" "       { }
"\n"      { }
"\t"      { }
{other}   { fprintf(stderr, "Illigal char '%c' ignored\n", yytext[0]);}

%%

int yywrap()
{
	return 1;
}

