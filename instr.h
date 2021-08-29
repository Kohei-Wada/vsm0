#ifndef INSTR_H
#define INSTR_H

typedef enum {
	NOP,
	ASSGN,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	CSIGN,
	AND,
	OR,
	NOT,
	COMP,
	COPY,
	PUSH,
	PUSHI,
	REMOVE,
	POP,
	INC,
	DEC,
	SETFR,
	INCFR,
	DECFR,
	JUMP,
	BLT,
	BLE,
	BEQ,
	BNE,
	BGE,
	BGT,
	CALL,
	RET,
	HALT,
	INPUT,
	OUTPUT
} op_t;

typedef struct instr instr_t;

void instr_set_op(instr_t *i, op_t op);
op_t instr_get_op(instr_t *i);

void instr_set_reg(instr_t *i, unsigned int reg);
unsigned int instr_get_reg(instr_t *i);

void instr_set_addr(instr_t *i, int addr);
int instr_get_addr(instr_t *i);

void instr_display(instr_t *i);

int instr_init(instr_t **i);
int instr_free(instr_t *i);


#endif
