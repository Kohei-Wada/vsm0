#include <stdio.h>
#include <stdlib.h>

#include "vsm.h"
#include "instr.h"
#include "stack.h"


typedef struct vsm {
	int pc;          // program counter
	int freg;        // flag register
	instr_t **iseg;  // instruction segment
	int *dseg;       // data segment

	stack_t *stack;  // stack object

	int debug;
	int halt;

	int ins_count;
	int max_sd;
	int min_fr;
	int max_pc;
	int call_c;
} vsm_t;


static void vsm_print_instr(vsm_t *v, int loc);

void vsm_set_debug(vsm_t *v, int debug);
int vsm_is_debug(vsm_t *v);

void vsm_set_pc(vsm_t *v, int addr);
int vsm_get_pc(vsm_t *v);
void vsm_inc_pc(vsm_t *v);

void vsm_set_sp(vsm_t *v, int addr);
int vsm_get_sp(vsm_t *v);

void vsm_set_freg(vsm_t *v, int flag);
int vsm_get_freg(vsm_t *v);

void vsm_set_halt(vsm_t *v, int flag);
int vsm_get_halt(vsm_t *v);

static instr_t* vsm_get_instr(vsm_t *v, int pc);
void vsm_set_instr(vsm_t *v, int pc, op_t op, int flag, int addr);

static void vsm_set_dseg(vsm_t *v, int addr, int val);
static int vsm_get_dseg(vsm_t *v, int addr);

static stack_t *vsm_get_stack(vsm_t *v);

int vsm_start(vsm_t *v, int start_addr, int trace_flag);

int vsm_back_patching(vsm_t *v, int loc, int target);

int vsm_init(vsm_t **v);
int vsm_free(vsm_t *v);

void vsm_dump_iseg(vsm_t *v, int first, int last);

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

/*XXX*/
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

static int vsm_handle_instr(vsm_t *v, int pc);




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

void vsm_inc_pc(vsm_t *v)
{
	++v->pc;
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


static stack_t *vsm_get_stack(vsm_t *v)
{
	return v->stack;
}


static void vsm_set_dseg(vsm_t *v, int addr, int val)
{
	v->dseg[addr] = val;
}


static int vsm_get_dseg(vsm_t *v, int addr)
{
	return v->dseg[addr];
}


void vsm_set_halt(vsm_t *v, int flag)
{
	v->halt = flag;
}


int vsm_get_halt(vsm_t *v)
{
	return v->halt;
}


static void vsm_print_instr(vsm_t *v, int loc)
{
	instr_t *i = vsm_get_instr(v, loc);
	instr_display(i);
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


static int vsm_handle_instr(vsm_t *v, int pc)
{
	instr_t *i = vsm_get_instr(v, pc);

	switch (instr_get_op(i)) {
	case NOP    : vsm_handle_nop(v);    break ;
	case ASSGN  : vsm_handle_assgn(v);  break ;
	case ADD    : vsm_handle_add(v);    break ;
	case SUB    : vsm_handle_sub(v);    break ;
	case MUL    : vsm_handle_mul(v);    break ;
	case DIV    : vsm_handle_div(v);    break ;
	case MOD    : vsm_handle_mod(v);    break ;
	case CSIGN  : vsm_handle_csign(v);  break ;
	case AND    : vsm_handle_and(v);    break ;
	case OR     : vsm_handle_or(v);     break ;
	case NOT    : vsm_handle_not(v);    break ;
	case COMP   : vsm_handle_comp(v);   break ;
	case COPY   : vsm_handle_copy(v);   break ;
	case PUSH   : vsm_handle_push(v);   break ;
	case PUSHI  : vsm_handle_pushi(v);  break ;
	case REMOVE : vsm_handle_remove(v); break ;
	case POP    : vsm_handle_pop(v);    break ;
	case INC    : vsm_handle_inc(v);    break ;
	case DEC    : vsm_handle_dec(v);    break ;
	case SETFR  : vsm_handle_setfr(v);  break ;
	case INCFR  : vsm_handle_incfr(v);  break ;
	case DECFR  : vsm_handle_decfr(v);  break ;
	case JUMP   : vsm_handle_jump(v);   break ;
	case BLT    : vsm_handle_blt(v);    break ;
	case BLE    : vsm_handle_ble(v);    break ;
	case BEQ    : vsm_handle_beq(v);    break ;
	case BNE    : vsm_handle_bne(v);    break ;
	case BGE    : vsm_handle_bge(v);    break ;
	case BGT    : vsm_handle_bgt(v);    break ;
	case CALL   : vsm_handle_call(v);   break ;
	case RET    : vsm_handle_ret(v);    break ;
	case HALT   : vsm_handle_halt(v);   break ;
	case INPUT  : vsm_handle_input(v);  break ;
	case OUTPUT : vsm_handle_output(v); break ;
	default: break;
	}

	return 0;
}


int vsm_start(vsm_t *v, int start_addr, int trace_sw)
{
	vsm_set_pc(v, start_addr);
	vsm_set_freg(v, 0);

	while (!vsm_get_halt(v)) {
		vsm_handle_instr(v, vsm_get_pc(v));
		vsm_inc_pc(v);
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
	stack_init(&(*v)->stack);

	if (!(*v)->iseg || !(*v)->dseg || !(*v)->stack) 
		goto vsm_error_malloc;

	vsm_set_pc(*v, 0);

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
	free(v->stack);
	free(v->dseg);
	free(v);
	return 0;
}


void vsm_dump_iseg(vsm_t *v, int first, int last)
{
	printf("\nContents of Instruction Segment\n");
	for (; first <= last; ++first) 
		vsm_print_instr(v, first);

	printf("\n");
}


static void vsm_handle_nop(vsm_t *v)
{
	return;
}


static void vsm_handle_assgn(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int addr = stack_pop(s);
	stack_pop(s);
	stack_push(s, addr);

	vsm_set_dseg(v, addr, addr);
}


static void vsm_handle_add(vsm_t *v) 
{
	stack_t *s = vsm_get_stack(v);

	int val2 = stack_pop(s);
	int val1 = stack_pop(s);

	stack_push(s, val1 + val2);
}


static void vsm_handle_sub(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val2 = stack_pop(s);
	int val1 = stack_pop(s);

	stack_push(s, val1 - val2);
}


static void vsm_handle_mul(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val2 = stack_pop(s);
	int val1 = stack_pop(s);

	stack_push(s, val1 * val2);
}


static void vsm_handle_div(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val2 = stack_pop(s);
	int val1 = stack_pop(s);

	stack_push(s, val1 / val2);
}


static void vsm_handle_mod(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val2 = stack_pop(s);
	int val1 = stack_pop(s);

	stack_push(s, val1 % val2);
}


static void vsm_handle_push(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);
	instr_t *i = vsm_get_instr(v, vsm_get_pc(v));

	int addr = instr_get_addr(i);
	int val  = vsm_get_dseg(v, addr);

	stack_push(s, val);
}


static void vsm_handle_csign(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val = stack_pop(s);
	stack_push(s, -val);
}


static void vsm_handle_and(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val1 = stack_pop(s);
	int val2 = stack_pop(s);

	stack_push(s, val1 && val2);
}


static void vsm_handle_or(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val1 = stack_pop(s);
	int val2 = stack_pop(s);

	stack_push(s, val1 || val2);
}


static void vsm_handle_not(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val = stack_pop(s);

	stack_push(s, !val);
}


static void vsm_handle_comp(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int next = stack_pop(s);
	int first = stack_pop(s);

	int retval;
	if (first > next) 
		retval = 1;
	else if (first < next)
		retval = -1;
	else 
		retval = 0;

	stack_push(s, retval);
}


static void vsm_handle_copy(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val = stack_pop(s);

	stack_push(s, val);
	stack_push(s, val);
}


static void vsm_handle_pushi(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);

	int addr = instr_get_addr(i);

	stack_push(s, addr);
}


static void vsm_handle_remove(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);
	stack_pop(s);
}


static void vsm_handle_pop(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);
	instr_t *i = vsm_get_instr(v, vsm_get_pc(v));

	int addr = instr_get_addr(i);
	int val = stack_pop(s);

	vsm_set_dseg(v, addr, val);
}


static void vsm_handle_inc(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val1 = stack_pop(s);

	stack_push(s, val1 + 1);

}

static void vsm_handle_dec(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int val1 = stack_pop(s);

	stack_push(s, val1 - 1);
}


static void vsm_handle_setfr(vsm_t *v)
{
	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);
	vsm_set_freg(v, addr);
}


/*XXX*/
static void vsm_handle_incfr(vsm_t *v)
{

}


/*XXX*/
static void vsm_handle_decfr(vsm_t *v)
{

}


static void vsm_handle_jump(vsm_t *v)
{
	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	vsm_set_pc(v, addr);
}

static void vsm_handle_blt(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val < 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_ble(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val <= 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_beq(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val == 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_bne(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val != 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_bge(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val >= 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_bgt(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	int val = stack_pop(s);

	if (val > 0)
		vsm_set_pc(v, addr);
}


static void vsm_handle_call(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);

	int pc = vsm_get_pc(v);
	stack_push(s, pc);

	instr_t *i = vsm_get_instr(v, pc);
	int addr = instr_get_addr(i);

	vsm_set_pc(v, addr);
}


static void vsm_handle_ret(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);
	vsm_set_pc(v, stack_pop(s));
}


static void vsm_handle_halt(vsm_t *v)
{
	vsm_set_halt(v, 1);
}


static void vsm_handle_input(vsm_t *v)
{
	int val;
	stack_t *s = vsm_get_stack(v);
	scanf("%d", &val);
	int addr = stack_pop(s);
	vsm_set_dseg(v, addr, val);
}


static void vsm_handle_output(vsm_t *v)
{
	stack_t *s = vsm_get_stack(v);
	int val = stack_pop(s);
	printf("%15d\n", val);
}

