#ifndef _VSM_H_
#define _VSM_H_


#define ISEG_SIZE 1000
#define DSEG_SIZE 1000
#define FRAME_BOTTOM (DSEG_SIZE - 1)


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

#endif 
