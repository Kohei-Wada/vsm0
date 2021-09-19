#include <stdlib.h>

#include "label.h"
#include "parse.h"
#include "vsm.h"


void yyerror(const char *);

#define CT_SIZE 100

typedef struct {
	int label;
	int addr;
} CSentry;

static CSentry Cstk[CT_SIZE], *casep = Cstk;


#define SWT_SIZE 10
struct {
	CSentry *cptr;
	int default_addr;
} switch_table[SWT_SIZE];

static int swt_ptr = 0;



void begin_switch(void)
{
	switch_table[++swt_ptr].cptr = casep;
	switch_table[swt_ptr].default_addr = -1;
}


void case_label(int clabel, parser_t *p)
{
	CSentry *entry;

	if (swt_ptr > 0) {
		casep->label = clabel;
		for (entry = switch_table[swt_ptr].cptr; entry->label != clabel; ++entry);
		if (entry >= casep) {
			casep->addr = parser_get_pc(p);
			++casep;
		}
		else
			yyerror("Duplicated case constant");
	}
}



void default_label(parser_t *p)
{
	if (swt_ptr > 0 && switch_table[swt_ptr].default_addr < 0) 
		switch_table[swt_ptr].default_addr = parser_get_pc(p);
	else
		yyerror("Illegal default label.");
}


void end_switch(parser_t *p)
{
	CSentry *entry;
	int pc;

	for (entry = switch_table[swt_ptr].cptr; entry < casep; ++entry) {
		pc = parser_get_pc(p);

		parser_set_instr(p, pc, COPY, 0, 0);
		parser_set_instr(p, pc+1, PUSHI, 0, entry->label);
		parser_set_instr(p, pc+2, COMP, 0, 0);
		parser_set_instr(p, pc+3, BNE, 0, pc+6);
		parser_set_instr(p, pc+4, REMOVE, 0, 0);
		parser_set_instr(p, pc+5, JUMP, 0, entry->addr);
	
		parser_set_pc(p, pc + 6);
	}

	pc = parser_get_pc(p);
	parser_set_instr(p, pc, REMOVE, 0, 0);
	parser_inc_pc(p);


	if (switch_table[swt_ptr].default_addr > 0) {
		pc = parser_get_pc(p);
		parser_set_instr(p, pc, JUMP, 0, switch_table[swt_ptr].default_addr);
		parser_inc_pc(p);
	}

	casep = switch_table[swt_ptr--].cptr;
}

