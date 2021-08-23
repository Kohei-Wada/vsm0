#include <stdio.h>
#include <stdlib.h>
#include "vsm.h"

typedef struct instr {
	unsigned int op;    // operation
	unsigned int reg;   // register
	int addr;           // address
} instr_t;

typedef struct vsm {
	int pcounter;    // program counter
	int sp;          // stack pointer
	instr_t *iseg;   // instruction segment
	int *dseg;       // data segment

	int debug;

	int ins_count;
	int max_sd;
	int min_fr;
	int max_pc;
	int call_c;
} vsm_t;


static void vsm_print_instruction(vsm_t *v, int loc);

void vsm_set_debug(vsm_t *v, int debug);
int vsm_is_debug(vsm_t *v);

void vsm_set_pcounter(vsm_t *v, int addr);
int vsm_get_pcounter(vsm_t *v);

int vsm_start(vsm_t *v, int start_addr, int trace_flag);

//void vsm_set_iseg();
//void vms_dump_iseg();

//void vsm_exec_report();

int vsm_back_patching(vsm_t *v, int loc, int target);

int vsm_init(vsm_t **v);
void fsm_free(vsm_t *v);


static char *scode[] = {
	"Nop", "  =" , "  +", "  -","  *", "  /", "  %",
	"  -'", "and", "or", "not", "comp", "copy", "push",
	"push-i", "remove", "pop", " ++", " --", "setFR",
	"++FR", "--FR", "jump", "<0 ?", "<=0 ?", "==0 ?",
	"!=0 ?", ">=0 ?", ">0 ?", "call", "return",
	"halt", "input", "output",
};

static void vsm_print_instruction(vsm_t *v, int loc)
{
	int op = v->iseg[loc].op;
	printf("%d, %s\n", loc, scode[op]);

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
		printf("%6d%4s", v->iseg[loc].addr, v->iseg[loc].reg ? "[fp]" : " ");
	default:
		printf("%10c", ' ');
	}
}


void vsm_set_debug(vsm_t *v, int debug)
{
	v->debug = debug;
}

int vsm_is_debug(vsm_t *v) 
{
	return v->debug;
}

void vsm_set_pcounter(vsm_t *v, int addr)
{
	v->pcounter = addr;
}

int vsm_get_pcounter(vsm_t *v)
{
	return v->pcounter;
}


void vsm_set_instruction(vsm_t *v, op_t op, int flag, int addr)
{
	int pc = v->pcounter;

	v->iseg[pc].op = op;
	v->iseg[pc].reg = flag;
	v->iseg[pc].addr = addr;
}

int vsm_back_patching(vsm_t *v, int loc, int target)
{
	int p;

	while (loc >= 0) {
		p = v->iseg[loc].addr;

		if (p == loc) {
			printf("trying to rewrite self address part at loc. %d\n", p);
			return 0;
		}
		v->iseg[loc].addr = target;
		loc = p;
	}
	return 0;
}


int vsm_start(vsm_t *v, int start_addr, int trace_sw)
{
	return 0;
}

int vsm_init(vsm_t **v)
{
	(*v) = malloc(sizeof(vsm_t));
	(*v)->iseg = malloc(sizeof(int) * ISEG_SIZE);
	(*v)->dseg = malloc(sizeof(int) * DSEG_SIZE);

	if (!(*v)->iseg || !(*v)->dseg) 
		return 1;

	(*v)->pcounter = 0;
	(*v)->sp       = 0;

	return 0;
}

int vsm_free(vsm_t *v)
{
	free(v->iseg);
	free(v->dseg);
	free(v);

	return 0;
}

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_debug(vsm, 1);
	printf("vsm_is_debug = %d\n", vsm_is_debug(vsm));

	vsm_start(vsm, 0, 0);
	vsm_free(vsm);

	return 0;
}
