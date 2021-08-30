%{
/*include instr , befor y.tab.h*/
#include "instr.h"
#include "y.tab.h"
%}


letter [a-zA-Z_]

id     {letter}({letter}|[0-9])*

integer 0|[1-9][0-9]*
symbol [();!]
other .


%%

{integer} { yylval.int_value = atoi(yytext); return NUM; }

{symbol}  { return yytext[0];}

"+"       { yylval.op = ADD; return ADDOP; }
"-"       { yylval.op = SUB; return ADDOP; }
"*"       { yylval.op = MUL; return MULOP; }
"/"       { yylval.op = DIV; return MULOP; }
"%"       { yylval.op = MOD; return MULOP; }
"||"      { yylval.op = OR;  return LOR; }
"&&"      { yylval.op = AND; return LAND; }

" "       { }
"\n"      { }
"\t"      { }
{other}   { fprintf(stderr, "Illigal char '%c' ignored\n", yytext[0]);}

%%

int yywrap()
{
	return 1;
}

