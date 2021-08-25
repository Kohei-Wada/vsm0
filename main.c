#include "vsm.h"
#include "test.c"

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_debug(vsm, 1);

	test3(vsm);

	//vsm_dump_iseg(vsm, 0, 11);

	vsm_start(vsm, 0, 0);
	vsm_free(vsm);

	return 0;
}