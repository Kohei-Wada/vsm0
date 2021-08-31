#include <stdlib.h> 
#include "symtable.h"


typedef struct symtable{
	char *table[SYMTABLE_SIZE];
	int last;
} symtable_t;


static char **symtable_get_table(symtable_t *s)
{
	return s->table;
}


static int symtable_get_last(symtable_t *s)
{
	return s->last;
}


static void symtable_set_last(symtable_t *s, int value)
{
	s->last = value;
}


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


int symtable_decl(symtable_t *s, char *name)
{
	int i;
	char **table = symtable_get_table(s);
	int last = symtable_get_last(s);

	table[last + 1] = name;

	for (i = 1; table[i] != name ; ++i);

	if (i > last)
		symtable_set_last(s, i);
	
	return 0;
}


int symtable_ref(symtable_t *s, char *name)
{
	int i;
	char **table = symtable_get_table(s);
	int last = symtable_get_last(s);

	table[last + 1] = name;

	for (i = 1; table[i] != name; ++i);

	if (i > last)
		return -1;

	return i;
}
