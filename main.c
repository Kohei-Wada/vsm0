#include <time.h>
#include <stdlib.h>

#include "vsm.h"
#include "test.c"

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_debug(vsm, 1);

	srand(time(NULL));
	op_test(vsm, ADD);
	vsm_start(vsm, 0, 0);

	op_test(vsm, SUB);
	vsm_start(vsm, 0, 0);

	op_test(vsm, MUL);
	vsm_start(vsm, 0, 0);

	op_test(vsm, DIV);
	vsm_start(vsm, 0, 0);

	op_test(vsm, MOD);
	vsm_start(vsm, 0, 0);

	vsm_free(vsm);
	return 0;
}
