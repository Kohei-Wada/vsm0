#include <stdlib.h>
#include "instr.h"


typedef struct instr {
	op_t op;            // operation
	unsigned int reg;   // register
	int addr;           // address
} instr_t;


void instr_set_op(instr_t *i, op_t op)
{
	i->op = op;
}

op_t instr_get_op(instr_t *i)
{
	return i->op;
}


void instr_set_reg(instr_t *i, unsigned int reg)
{
	i->reg = reg;
}

unsigned int instr_get_reg(instr_t *i)
{
	return i->reg;
}


void instr_set_addr(instr_t *i, int addr)
{
	i->addr = addr;
}

int instr_get_addr(instr_t *i)
{
	return i->addr;
}


int instr_init(instr_t **i)
{
	*i = malloc(sizeof(instr_t));
	return 0;
}

void instr_free(instr_t *i)
{
	free(i);
}

