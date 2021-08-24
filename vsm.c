#include <stdio.h>
#include <stdlib.h>
#include "vsm.h"
#include "instr.h"

#define STACK_SIZE 100

typedef struct vsm {
	int pc;          // program counter
	int sp;          // stack pointer
	int freg;        // flag register
	instr_t **iseg;  // instruction segment
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

void vsm_set_pc(vsm_t *v, int addr);
int vsm_get_pc(vsm_t *v);

void vsm_set_sp(vsm_t *v, int addr);
int vsm_get_sp(vsm_t *v);

void vsm_set_freg(vsm_t *v, int flag);
int vsm_get_freg(vsm_t *v);

static void vsm_set_dseg(vms_t *v, int val, int addr);
static int vsm_get_dseg(vsm_t *v, int addr);

int vsm_start(vsm_t *v, int start_addr, int trace_flag);

static instr_t* vsm_get_instr(vsm_t *v, int pc);
void vsm_set_instr(vsm_t *v, int pc, op_t op, int flag, int addr);

int vsm_back_patching(vsm_t *v, int loc, int target);

int vsm_init(vsm_t **v);
int vsm_free(vsm_t *v);

//void vms_dump_iseg();
//void vsm_exec_report();

static void vsm_handle_nop(vsm_t *v);
static void vsm_handle_assgn(vsm_t *v);
static void vsm_handle_add(vsm_t *v);
static void vsm_handle_sub(vsm_t *v);
static void vsm_handle_mul(vsm_t *v);
static void vsm_handle_div(vsm_t *v);
static void vsm_handle_mod(vsm_t *v);
static void vsm_handle_csign(vsm_t *v);
static void vsm_handle_and(vsm_t *v);
static void vsm_handle_or(vsm_t *v);
static void vsm_handle_not(vsm_t *v);
static void vsm_handle_comp(vsm_t *v);
static void vsm_handle_copy(vsm_t *v);
static void vsm_handle_push(vsm_t *v);
static void vsm_handle_pushi(vsm_t *v);
static void vsm_handle_remove(vsm_t *v);
static void vsm_handle_pop(vsm_t *v);
static void vsm_handle_inc(vsm_t *v);
static void vsm_handle_dec(vsm_t *v);
static void vsm_handle_setfr(vsm_t *v);
static void vsm_handle_incfr(vsm_t *v);
static void vsm_handle_decfr(vsm_t *v);
static void vsm_handle_jump(vsm_t *v);
static void vsm_handle_blt(vsm_t *v);
static void vsm_handle_ble(vsm_t *v);
static void vsm_handle_beq(vsm_t *v);
static void vsm_handle_bne(vsm_t *v);
static void vsm_handle_bge(vsm_t *v);
static void vsm_handle_bgt(vsm_t *v);
static void vsm_handle_call(vsm_t *v);
static void vsm_handle_ret(vsm_t *v);
static void vsm_handle_halt(vsm_t *v);
static void vsm_handle_input(vsm_t *v);
static void vsm_handle_output(vsm_t *v);



static void vsm_print_instr(vsm_t *v, int loc)
{
	printf("loc = %d\n", loc);
	instr_t *i = vsm_get_instr(v, loc);
	instr_display(i);
}


void vsm_set_debug(vsm_t *v, int debug)
{
	v->debug = debug;
}


int vsm_is_debug(vsm_t *v) 
{
	return v->debug;
}


void vsm_set_pc(vsm_t *v, int addr)
{
	v->pc = addr;
}


int vsm_get_pc(vsm_t *v)
{
	return v->pc;
}


void vsm_set_freg(vsm_t *v, int addr)
{
	v->freg = addr;
}


int vsm_get_freg(vsm_t *v)
{
	return v->freg;
}


void vsm_set_sp(vsm_t *v, int addr)
{
	v->sp = addr;
}


int vsm_get_sp(vsm_t *v)
{
	return v->sp;
}

static void vsm_set_dseg(vms_t *v, int val, int addr)
{
	v->dseg[addr] = val;
}


static int vsm_get_dseg(vsm_t *v, int addr)
{
	return v->dseg[addr];
}


void vsm_set_instr(vsm_t *v, int pc, op_t op, int flag, int addr)
{
	instr_t *i = vsm_get_instr(v, pc);

	instr_set_op(i, op);
	instr_set_reg(i, flag);
	instr_set_addr(i, addr);

	if (vsm_is_debug(v))
		vsm_print_instr(v, pc);
}


static instr_t* vsm_get_instr(vsm_t *v, int pc)
{
	return v->iseg[pc];
}


int vsm_back_patching(vsm_t *v, int loc, int target)
{
	int p;
	instr_t *i;

	while (loc >= 0) {
		i = vsm_get_instr(v, loc);
		p = instr_get_addr(i);

		if (p == loc) {
			printf("trying to rewrite self address part at loc. %d\n", p);
			return 0;
		}

		instr_set_addr(i, target);
		loc = p;
	}
	return 0;
}


static int vsm_handle_oparations(vsm_t *v, op_t op)
{
	return 0;
}


int vsm_start(vsm_t *v, int start_addr, int trace_sw)
{
	instr_t *i;
	op_t op;
	int addr;
	int pc = start_addr;

	vsm_set_pc(v, start_addr);
	vsm_set_sp(v, 0);
	vsm_set_freg(v, 0);

	while (1) {

		i    = vsm_get_instr(v, pc);
		op   = instr_get_op(i);
		addr = instr_get_addr(i);

		vsm_handle_oparations(v, op);

		break;

	}

	return 0;
}


int vsm_init(vsm_t **v)
{
	(*v) = malloc(sizeof(vsm_t));
	
	if (!(*v)) 
		goto vsm_error_malloc;

	(*v)->iseg = malloc(sizeof(instr_t*) * ISEG_SIZE);

	for (int i = 0; i < ISEG_SIZE - 1; ++i)
		if (instr_init(&(*v)->iseg[i]))
			goto vsm_error_iseg_init;

	(*v)->dseg = malloc(sizeof(int) * DSEG_SIZE);

	if (!(*v)->iseg || !(*v)->dseg) 
		goto vsm_error_malloc;

	vsm_set_pc(*v, 0);
	vsm_set_sp(*v, 0);

	return 0;


  vsm_error_malloc:

	perror("mallc");

  vsm_error_iseg_init:

	return 1;
}


int vsm_free(vsm_t *v)
{
	for (int i = 0; i < ISEG_SIZE - 1; ++i)
		free(v->iseg[i]);

	free(v->iseg);

	free(v->dseg);
	free(v);
	return 0;
}


static void vsm_handle_nop(vsm_t *v)
{
	return;
}

static void vsm_handle_assgn(vsm_t *v)
{

}


static void vsm_handle_add(vsm_t *v) 
{


}

static void vsm_handle_sub(vsm_t *v)
{

}

static void vsm_handle_mul(vsm_t *v)
{


}

static void vsm_handle_div(vsm_t *v)
{

}

static void vsm_handle_mod(vsm_t *v)
{


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
