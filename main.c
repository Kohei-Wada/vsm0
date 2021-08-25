#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "vsm.h"
#include "test.c"

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_debug(vsm, 1);

	op_test(vsm);

	srand(time(NULL));
	vsm_free(vsm);
	return 0;
}
