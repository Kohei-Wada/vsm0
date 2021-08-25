#include "vsm.h"
#include "test.c"

int main(void) 
{
	vsm_t *vsm;

	vsm_init(&vsm);
	vsm_set_debug(vsm, 1);

	test4(vsm);

	//vsm_dump_iseg(vsm, 0, 11);
	
	vsm_exec_report(vsm);

	vsm_start(vsm, 0, 0);
	vsm_free(vsm);

	return 0;
}
