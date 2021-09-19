#include <stdlib.h>
#include "jmpchain.h"
#include "parse.h"
#include "instr.h"

void yyerror(const char *);

typedef struct chain_node {
	int stype;
	int bchain;
	int cchain;
} chain_node_t;

typedef struct jmpchain {
	int csptr;
	chain_node_t array[JMPCHAIN_SIZE];
	parser_t *p;
} jmpchain_t;


int jmpchain_get_csptr(jmpchain_t *j)
{
	return j->csptr;
}


static chain_node_t* jmpchain_get_node(jmpchain_t *j, int sptr)
{
	return &j->array[sptr];
}


static void jmpchain_set_node(jmpchain_t *j, chain_node_t *n)
{
	j->array[j->csptr] = *n;
}


void jmpchain_nestin(jmpchain_t *j, int st)
{
	j->csptr++;

	chain_node_t n;
	n.stype = st;
	n.bchain = -1;
	n.cchain = -1;
	jmpchain_set_node(j, &n);
}


void jmpchain_break(jmpchain_t *j, int jc)
{
	parser_t *p = j->p;

	if (j->csptr > 0) {
		parser_set_instr(p, parser_get_pc(p), jc, 0, j->array[j->csptr].bchain);
		parser_inc_pc(p);
		j->array[j->csptr].bchain = parser_get_pc(p) - 1;
	}
	else
		yyerror("Illigal use of break statement");
}



void jmpchain_nestout(jmpchain_t *j, int contp)
{
	parser_t *p = j->p;
	parser_back_patching(p, j->array[j->csptr].cchain, contp);
	parser_back_patching(p, j->array[j->csptr].bchain, parser_get_pc(j->p));
	j->csptr--;
}


void jmpchain_conti(jmpchain_t *j)
{
	parser_t *p = j->p;
	int i;

	for (i = j->csptr; i > 0 && j->array[i].stype == 0; i--);

	if (i <= 0)
		yyerror("Illegal use of the continue");
	else {
		int pc = parser_get_pc(p);
		parser_set_instr(p, pc, JUMP, 0, j->array[i].cchain);
		j->array[i].cchain = pc;
		parser_inc_pc(j->p);
	}
}


int jmpchain_init(jmpchain_t **j, parser_t *p)
{
	*j = malloc(sizeof(jmpchain_t));
	(*j)->csptr = 0;
	(*j)->p = p;
}


void jmpchain_free(jmpchain_t *j)
{
	free(j);
}
