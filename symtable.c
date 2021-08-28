#include <stdlib.h> 


typedef struct symtable{
	int i;
} symtable_t;

int symtable_init(symtable_t **s)
{
	*s = malloc(sizeof(symtable_t));

	if (!(*s))
		return 1;

	return 0;
}


void symtable_free(symtable_t *s)
{
	free(s);
}



