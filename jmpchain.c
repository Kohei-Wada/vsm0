#include <stdlib.h>
#include "jmpchain.h"
#include "parse.h"
#include "vsm.h"

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


void jmpchain_nestin(jmpchain_t *j, int st)
{
	j->csptr++;
	j->array[j->csptr].stype = st;
	j->array[j->csptr].bchain = -1;
	j->array[j->csptr].cchain = -1;
}


void jmpchain_break(jmpchain_t *j, int jc)
{
	vsm_t *v = parser_get_vsm(j->p);
	int pc = parser_get_pc(j->p);

	if (j->csptr > 0) {
		vsm_set_instr(v, pc, jc, 0, j->array[j->csptr].bchain);
		parser_inc_pc(j->p);

		j->array[j->csptr].bchain = parser_get_pc(j->p) - 1;
	}
	else
		yyerror("Illigal use of break statement");
}



void jmpchain_nestout(jmpchain_t *j, int contp)
{
	vsm_t *v = parser_get_vsm(j->p);
	vsm_back_patching(v, j->array[j->csptr].cchain, contp);
	vsm_back_patching(v, j->array[j->csptr].bchain, parser_get_pc(j->p));
	j->csptr--;
}


void jmpchain_conti(jmpchain_t *j)
{
	vsm_t *v = parser_get_vsm(j->p);
	int pc = parser_get_pc(j->p);
	int i;

	for (i = j->csptr; i > 0 && j->array[i].stype == 0; i--);

	if (i <= 0)
		yyerror("Illegal use of the continue");
	else {
		vsm_set_instr(v, pc, JUMP, 0, j->array[i].cchain);
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
