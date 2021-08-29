
#include <stdlib.h> 

#include "parse.h"
#include "nmtable.h"
#include "symtable.h"
#include "instr.h"
#include "vsm.h"

void set_yyvsm(vsm_t *v);
void yyparse();


typedef struct parser{
	nmtable_t *nmtable;
	symtable_t *symtable;
	vsm_t *vsm;
	int pc;
} parser_t;


void parser_set_vsm(parser_t *p, vsm_t *v)
{
	p->vsm = v;
}


vsm_t* parser_get_vsm(parser_t *p)
{
	return p->vsm;
}


static void parser_inc_pc(parser_t *p)
{
	++(p->pc);
}


static int parser_get_pc(parser_t *p)
{
	return p->pc;
}


int parser_init(parser_t **p, vsm_t *v)
{
	*p = malloc(sizeof(parser_t));

	if (!(*p))
		return 1;

	nmtable_init(&(*p)->nmtable);
	symtable_init(&(*p)->symtable);
	(*p)->vsm = v;

	set_yyvsm(v);

	return 0;
}


void parser_free(parser_t *p)
{
	nmtable_free(p->nmtable);
	symtable_free(p->symtable);
	free(p);
}


void parser_read(parser_t *p)
{
	yyparse();
}


void parser_handle_simple_op(parser_t *p, op_t op)
{
	vsm_t *v = parser_get_vsm(p);
	int pc = parser_get_pc(p);
	vsm_set_instr(v, pc, op, 0, 0); 
	parser_inc_pc(p);
}


