#include <string.h>
#include "vsm.h"

int main(int argc, char **argv) 
{

	int retval = 0;

	vsm_t *v;
	vsm_init(&v);

	if (argc > 1) {
		if (strcmp("-t", argv[1]) == 0)
			vsm_set_trace(v, 1);
	}

	parser_t *p = vsm_get_parser(v);
	if (retval = parser_read(p))
		goto fail;

	vsm_start(v, 0);


  fail:
	vsm_free(v);

	return retval;
}
