#include <stdlib.h> 


typedef struct parser{
	int i;
} parser_t;

int parser_init(parser_t **p)
{
	*p = malloc(sizeof(parser_t));

	if (!(*p))
		return 1;

	return 0;
}


void parser_free(parser_t *p)
{
	free(p);
}



