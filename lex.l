%{
#include <stdio.h>
#include "y.tab.h"
#include "vsm.h"
#include "instr.h"
%}

integer 0|[1-9][0-9]*
symbol [();\\n]
other .


%%

{integer} { yylval = atoi(yytext); return NUM; }

{symbol}  { return yytext[0];}

"+"       { yylval = ADD; return ADDOP; }
"-"       { yylval = SUB; return SUBOP; }
"*"       { yylval = MUL; return MULOP; }
"/"       { yylval = DIV; return DIVOP; }
"%"       { yylval = MOD; return MODOP; }
" "       { }

{other}   { fprintf(stderr, "Illigal char '%c' ignored\n", yytext[0]);}

%%

int yywrap()
{
	return 1;
}

