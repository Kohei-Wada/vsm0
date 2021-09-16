#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "parse.h"
#include "nmtable.h"
#include "symtable.h"
#include "instr.h"
#include "vsm.h"
#include "jmpchain.h"

int yyparse();
void yy_set_parser(parser_t *p);
void yy_set_yyin(FILE *f);

void yyl_set_parser(parser_t *p);



typedef struct parser{
	nmtable_t *nmtable;
	symtable_t *symtable;
	vsm_t *vsm;
	jmpchain_t *jchain;
	int pc;
	FILE *input_file;
} parser_t;


static void parser_set_vsm(parser_t *p, vsm_t *v)
{
	p->vsm = v;
}


vsm_t* parser_get_vsm(parser_t *p)
{
	return p->vsm;
}


jmpchain_t *parser_get_jchain(parser_t *p)
{
	return p->jchain;
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


int parser_get_pc(parser_t *p)
{
	return p->pc;
}



void parser_set_pc(parser_t *p, int value)
{
	p->pc = value;
}



int parser_read(parser_t *p)
{
	parser_set_pc(p, 0);
	yy_set_parser(p);
	yyl_set_parser(p);

	return yyparse();
}


void parser_handle_id(parser_t *p, op_t op, char *id_name)
{
	vsm_t *v = parser_get_vsm(p);
	int id_addr = parser_sym_ref(p, id_name);

	int pc = parser_get_pc(p);
	vsm_set_instr(v, pc, op, 0, id_addr); 
	parser_inc_pc(p);
}



int parser_sym_decl(parser_t *p, char *name, int init_value)
{
	int retval = 0;
	vsm_t *v = parser_get_vsm(p);
	symtable_t *s = parser_get_symtable(p);
	retval = symtable_decl(s, name);

	if (init_value && retval == 0) {
		int addr = symtable_ref(s, name);
		vsm_set_dseg(v, addr, init_value);
	}

	return retval;
}


int parser_sym_ref(parser_t *p, char *name)
{
	int id_addr;
	symtable_t *s = parser_get_symtable(p);
	id_addr = symtable_ref(s, name);

	/*if parser could not find id, declera id*/
	if (id_addr  < 0) {
		fprintf(stderr, "warning : '%s' is not declared\n", name);
		char *tmp = parser_id_entry(p, name, strlen(name));
		symtable_decl(s, tmp);
		id_addr = symtable_ref(s, name);
	}

	return id_addr;
}


int parser_init(parser_t **p, vsm_t *v)
{
	*p = malloc(sizeof(parser_t));

	if (!(*p))
		return 1;

	nmtable_init(&(*p)->nmtable);
	symtable_init(&(*p)->symtable);
	jmpchain_init(&(*p)->jchain, *p);

	(*p)->vsm = v;
	(*p)->pc = 0;
	(*p)->input_file = NULL;

	return 0;
}


void parser_free(parser_t *p)
{
	nmtable_free(p->nmtable);
	symtable_free(p->symtable);
	jmpchain_free(p->jchain);

	if (p->input_file)
		fclose(p->input_file);
	free(p);
}
/**************************************************************************/


char *parser_id_entry(parser_t *p, char *name, int len)
{
	nmtable_t *n = parser_get_nmtable(p);
	return nmtable_entry(n, name, len);
}

