#include <stdlib.h>
#include <stdio.h>

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


static char *scode[] = {
	"Nop", "  =" , "  +", "  -","  *", "  /", "  %",
	"  -'", "and", "or", "not", "comp", "copy", "push",
	"push-i", "remove", "pop", " ++", " --", "setFR",
	"++FR", "--FR", "jump", "<0 ?", "<=0 ?", "==0 ?",
	"!=0 ?", ">=0 ?", ">0 ?", "call", "return",
	"halt", "input", "output",
};

void instr_display(instr_t *i)
{
	unsigned int op = instr_get_op(i); 
	int addr = instr_get_addr(i);

	printf("%5d  %-8s", addr, scode[op]);

	switch (op) {
	case PUSH:
	case PUSHI:
	case POP:
	case SETFR:
	case INCFR:
	case DECFR:
	case JUMP:
	case BLT:
	case BLE:
	case BEQ:
	case BNE:
	case BGE:
	case BGT:
	case CALL:
		printf("%6d%4s", instr_get_addr(i), instr_get_reg(i) ? "[fp]" : " ");
	default:
		printf("%10c", ' ');
	}
	printf("\n");
}


int instr_init(instr_t **i)
{
	*i = malloc(sizeof(instr_t));
	
	if (!(*i))
		return 1;

	return 0;
}

int instr_free(instr_t *i)
{
	free(i);
	return 0;
}

