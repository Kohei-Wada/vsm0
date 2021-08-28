#include <stdlib.h> 


typedef struct nmtable{
	int i;
} nmtable_t;

int nmtable_init(nmtable_t **n)
{
	*n = malloc(sizeof(nmtable_t));

	if (!(*n))
		return 1;

	return 0;
}


void nmtable_free(nmtable_t *n)
{
	free(n);
}



