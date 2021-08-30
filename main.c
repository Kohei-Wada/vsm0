#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vsm.h"

int main(int argc, char **argv) 
{

	int retval = 0;
	int trace = 0;

	if (argc > 1) {
		if (strcmp("-t", argv[1]) == 0)
			trace = 1;
	}

	vsm_t *v;
	vsm_init(&v);
	vsm_set_trace(v, trace);

	parser_t *p = vsm_get_parser(v);
	if (retval = parser_read(p))
		goto fail;

	vsm_start(v, 0);


  fail:
	vsm_free(v);

	return 0;
}
