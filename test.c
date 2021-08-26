#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vsm.h"


static void nop_test(vsm_t *v)
{
	vsm_set_instr(v, 0, NOP, 0, 0);
	vsm_set_instr(v, 1, HALT, 0, 0);
}


static void assgn_test(vsm_t *v)
{

}


static void op_test(vsm_t *v)
{
	for (op_t op = ADD; op <= COPY; ++op) {
		vsm_set_instr(v, 0, PUSHI, 0, random()%1000);
		vsm_set_instr(v, 1, PUSHI, 0, random()%1000);
		vsm_set_instr(v, 2, op, 0, 0);
		vsm_set_instr(v, 3, OUTPUT, 0, 0);
		vsm_set_instr(v, 4, HALT, 0, 0);
		vsm_start(v, 0, 0);
		printf("\n");
	}
}


