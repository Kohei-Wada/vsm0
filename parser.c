#include <stdlib.h> 
#include "nmtable.h"
#include "symtable.h"


typedef struct parser{
	nmtable_t *nmtable;
	symtable_t *symtable;
} parser_t;


int parser_init(parser_t **p)
{
	*p = malloc(sizeof(parser_t));

	if (!(*p))
		return 1;

	nmtable_init(&(*p)->nmtable);
	symtable_init(&(*p)->symtable);

	return 0;
}


void parser_free(parser_t *p)
{
	nmtable_free(p->nmtable);
	symtable_free(p->symtable);
	free(p);
}



