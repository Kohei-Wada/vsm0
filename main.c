#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "vsm.h"
#include "y.tab.h"

int main(void) 
{

	vsm_t *v;
	vsm_init(&v);
//	vsm_set_trace(v, 1);

	parser_t *p = vsm_get_parser(v);
	parser_read(p);

	vsm_start(v, 0);


	vsm_free(v);

	return 0;
}
