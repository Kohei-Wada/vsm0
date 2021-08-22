#include <stdio.h>
#include <stdlib.h>


typedef struct vsm {
	int i;
} vsm_t;



void vsm_init(vsm_t **v)
{
	(*v) = malloc(sizeof(vsm_t));
}


void vsm_free(vsm_t *v)
{
	free(v);
}


int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_free(vsm);

	return 0;
}

