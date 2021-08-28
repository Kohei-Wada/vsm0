#include <stdlib.h> 
#include "symtable.h"


typedef struct symtable{
	char *table[SYMTABLE_SIZE];
	int last;
} symtable_t;

int symtable_init(symtable_t **s)
{
	*s = malloc(sizeof(symtable_t));

	if (!(*s))
		return 1;

	(*s)->last = 0;

	return 0;
}


void symtable_free(symtable_t *s)
{
	free(s);
}


int symtable_decl(symtable_t *s, char *symname)
{
	return 0;
}

