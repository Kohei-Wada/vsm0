%{
/*include instr , befor y.tab.h*/
#include "instr.h"
#include "y.tab.h"
#include "parse.h"
#include <stdio.h>
static parser_t *lp = NULL;
%}


letter [a-zA-Z_]

id     {letter}({letter}|[0-9])*

integer 0|[1-9][0-9]*
symbol [?:{}();!,=]
other .


%%

" "        { }
"\n"       { }
"\t"       { }
{symbol}   { return yytext[0];}
"+"        { yylval.op = ADD; return ADDOP; }
"-"        { yylval.op = SUB; return ADDOP; }
"*"        { yylval.op = MUL; return MULOP; }
"/"        { yylval.op = DIV; return MULOP; }
"%"        { yylval.op = MOD; return MULOP; }

"++"       { yylval.op = INC; return PPMM; }
"--"       { yylval.op = DEC; return PPMM; }

"||"       { yylval.op = OR;  return LOR; }
"&&"       { yylval.op = AND; return LAND; }

"<"        { yylval.op = BLT; return RELOP; }
"<="       { yylval.op = BLE; return RELOP; }
"=="       { yylval.op = BEQ; return RELOP; }
"!="       { yylval.op = BNE; return RELOP; }
">="       { yylval.op = BGE; return RELOP; }
">"        { yylval.op = BGT; return RELOP; }


"int"      { return TYPE; }
"for"      { return FOR; }
"if"       { return IF; }
"else"     { return ELSE; }
"while"    { return WHILE; }
"break"    { return BREAK; }
"continue" { return CONTI; }
"write"    { yylval.op = OUTPUT; return WRITE; }
"read"     { yylval.op = INPUT;  return READ; }

{integer}  { yylval.int_value = atoi(yytext); return NUM; }
{id}       { yylval.name = parser_id_entry(lp, yytext, yyleng); return ID; }

{other}    { fprintf(stderr, "Illigal char '%c' ignored\n", yytext[0]);}

%%

int yywrap()
{
	return 1;
}


void yyl_set_parser(parser_t *p)
{
	lp = p;
}
