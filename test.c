#include "vsm.h"


static void test0(vsm_t *v)
{
	vsm_set_instr(v, 1, PUSHI, 0, 20);
	vsm_set_instr(v, 2, PUSHI, 0, 30);
	vsm_set_instr(v, 3, ADD, 0, 0);
	vsm_set_instr(v, 4, PUSHI, 0, 3);
	vsm_set_instr(v, 5, PUSHI, 0, 8);
	vsm_set_instr(v, 6, SUB, 0, 3);
	vsm_set_instr(v, 7, DIV, 0, 0);
	vsm_set_instr(v, 8, OUTPUT, 0, 0);
	vsm_set_instr(v, 9, HALT, 0, 0);
}

static void test1(vsm_t *v)
{
	vsm_set_instr(v, 1,  PUSHI, 0, 20);
	vsm_set_instr(v, 2,  PUSHI, 0, 0);
	vsm_set_instr(v, 3,  INPUT, 0, 0);
	vsm_set_instr(v, 4,  PUSH, 0, 0);
	vsm_set_instr(v, 5,  ADD, 0, 0);
	vsm_set_instr(v, 6,  PUSHI, 0, 3);
	vsm_set_instr(v, 7,  PUSHI, 0, 8);
	vsm_set_instr(v, 8,  SUB, 0, 3);
	vsm_set_instr(v, 9,  DIV, 0, 0);
	vsm_set_instr(v, 10, OUTPUT, 0, 0);
	vsm_set_instr(v, 11, HALT, 0, 0);
}


static void test2(vsm_t *v)
{
	vsm_set_instr(v, 1,  PUSHI, 0, 15);
	vsm_set_instr(v, 2,  CSIGN, 0, 0);
	vsm_set_instr(v, 3,  PUSHI, 0, 63);
	vsm_set_instr(v, 4,  PUSHI, 0, 94);
	vsm_set_instr(v, 5,  ADD, 0, 0);
	vsm_set_instr(v, 6,  MUL, 0, 0);
	vsm_set_instr(v, 7,  OUTPUT, 0, 0);
	vsm_set_instr(v, 11, HALT, 0, 0);
}


static void test3(vsm_t *v)
{
	vsm_set_instr(v, 1,  PUSHI, 0, 401);
	vsm_set_instr(v, 2,  PUSHI, 0, 552);
	vsm_set_instr(v, 3,  ADD, 0, 0);
	vsm_set_instr(v, 4,  PUSHI, 0, 223);
	vsm_set_instr(v, 5,  PUSHI, 0, 158);
	vsm_set_instr(v, 6,  SUB, 0, 0);
	vsm_set_instr(v, 7,  DIV, 0, 0);
	vsm_set_instr(v, 8,  OUTPUT, 0, 0);
	vsm_set_instr(v, 9, HALT, 0, 0);
}



