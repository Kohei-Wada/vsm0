
#include <stdlib.h> 

#include "parser.h"
#include "nmtable.h"
#include "symtable.h"
#include "y.tab.h"


typedef struct parser{
	nmtable_t *nmtable;
	symtable_t *symtable;
	vsm_t *vsm;
} parser_t;


int parser_set(vsm_t *vsm)
{
	return 0;
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

