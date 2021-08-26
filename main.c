#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "vsm.h"
#include "test.c"

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_trace(vsm, 1);
	srand(time(NULL));

	op_test(vsm);

	vsm_free(vsm);
	return 0;
}
