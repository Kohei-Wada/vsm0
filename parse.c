#include <stdio.h>
#include <stdlib.h> 

#include "parse.h"
#include "nmtable.h"
#include "symtable.h"
#include "instr.h"
#include "vsm.h"

int yyparse();
void yy_set_parser(parser_t *p);
void yy_set_yyin(FILE *f);

void yyl_set_parser(parser_t *p);



typedef struct parser{
	nmtable_t *nmtable;
	symtable_t *symtable;
	vsm_t *vsm;
	int pc;
	FILE *input_file;
} parser_t;


static void parser_set_vsm(parser_t *p, vsm_t *v)
{
	p->vsm = v;
}


static vsm_t* parser_get_vsm(parser_t *p)
{
	return p->vsm;
}


static void parser_set_nmtable(parser_t *p, nmtable_t *n)
{
	p->nmtable = n;
}


static nmtable_t* parser_get_nmtable(parser_t *p)
{
	return p->nmtable;
}

static void parser_set_symtable(parser_t *p, symtable_t *s)
{
	p->symtable = s;
}


static symtable_t* parser_get_symtable(parser_t *p)
{
	return p->symtable;
}


void parser_inc_pc(parser_t *p)
{
	++(p->pc);
}


int parser_set_input_file(parser_t *p, const char *file_name)
{
	FILE *f = fopen(file_name, "r");
	if (!f)
		return 1;

	p->input_file = f;
	yy_set_yyin(f);

	return 0;
}


static int parser_get_pc(parser_t *p)
{
	return p->pc;
}


static void parser_set_pc(parser_t *p, int value)
{
	p->pc = value;
}


int parser_init(parser_t **p, vsm_t *v)
{
	*p = malloc(sizeof(parser_t));

	if (!(*p))
		return 1;

	nmtable_init(&(*p)->nmtable);
	symtable_init(&(*p)->symtable);

	(*p)->vsm = v;
	(*p)->pc = 0;
	(*p)->input_file = NULL;

	return 0;
}


void parser_free(parser_t *p)
{
	nmtable_free(p->nmtable);
	symtable_free(p->symtable);

	if (p->input_file)
		fclose(p->input_file);
	free(p);
}


int parser_read(parser_t *p)
{
	parser_set_pc(p, 0);
	yy_set_parser(p);
	yyl_set_parser(p);

	return yyparse();
}


void parser_handle_simple_op(parser_t *p, op_t op)
{
	vsm_t *v = parser_get_vsm(p);
	int pc = parser_get_pc(p);
	vsm_set_instr(v, pc, op, 0, 0); 
	parser_inc_pc(p);
}


void parser_handle_ppmm(parser_t *p, op_t op, char *id_name, int priorize)
{
	vsm_t *v = parser_get_vsm(p);
	int addr = parser_sym_ref(p, id_name);
	int pc = parser_get_pc(p);

	vsm_set_instr(v, pc, PUSH, 0, addr); 

	if (priorize) {
		vsm_set_instr(v, pc + 1, op, 0, 0); 
		vsm_set_instr(v, pc + 2, COPY, 0, 0); 
	}
	else {
		vsm_set_instr(v, pc + 1, COPY, 0, 0); 
		vsm_set_instr(v, pc + 2, op, 0, 0); 
	}
	vsm_set_instr(v, pc + 3, POP, 0, addr); 
	parser_set_pc(p, pc + 4);
}


void parser_handle_id(parser_t *p, op_t op, char *id_name)
{
	vsm_t *v = parser_get_vsm(p);
	int id_addr = parser_sym_ref(p, id_name);
	int pc = parser_get_pc(p);
	vsm_set_instr(v, pc, op, 0, id_addr); 
	parser_inc_pc(p);
}


void parser_handle_num(parser_t *p, int num)
{
	vsm_t *v = parser_get_vsm(p);
	int pc = parser_get_pc(p);

	vsm_set_instr(v, pc, PUSHI, 0, num); 
	parser_inc_pc(p);
}


void parser_handle_relop(parser_t *p, op_t op)
{
	vsm_t *v = parser_get_vsm(p);
	int pc = parser_get_pc(p);

	/*Since the program counter is incremented each time the instruction is executed, 
	 * set the JUMP instruction just befor the place where you want to JUMP.*/

	vsm_set_instr(v, pc, COMP, 0, 0); 
	vsm_set_instr(v, pc + 1, op, 0, pc + 3);  
	vsm_set_instr(v, pc + 2, PUSHI, 0, 0); 
	vsm_set_instr(v, pc + 3, JUMP, 0, pc + 4); 
	vsm_set_instr(v, pc + 4, PUSHI, 0, 1); 

	parser_set_pc(p, pc + 5);

}


int parser_sym_decl(parser_t *p, char *name)
{
	symtable_t *s = parser_get_symtable(p);
	return symtable_decl(s, name);
}


int parser_sym_ref(parser_t *p, char *name)
{
	symtable_t *s = parser_get_symtable(p);
	return symtable_ref(s, name);
}


/**************************************************************************/


char *parser_id_entry(parser_t *p, char *name, int len)
{
	nmtable_t *n = parser_get_nmtable(p);
	return nmtable_entry(n, name, len);
}

