#include <stdlib.h>


typedef struct jmpchain {
	int stype;
	int bchain;
	int cchain;
} jmpchain_t;



int jmpchain_init(jmpchain_t **j)
{
	*j = malloc(sizeof(jmpchain_t));
}


void jmpchain_free(jmpchain_t *j)
{
	free(j);
}
