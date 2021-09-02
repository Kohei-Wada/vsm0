#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "vsm.h"



int main(int argc, char **argv) 
{

	vsm_t *v;
	vsm_init(&v);


	parser_t *p = vsm_get_parser(v);


	int opt;
	while ((opt = getopt(argc, argv, "tf:")) != -1) {
		switch (opt){
		case 't':
			vsm_set_trace(v, 1);
			break;

		case 'f':
			if (parser_set_input_file(p, optarg))
				goto fail;
			break;
		default:
			goto fail;
		}
	}


	int retval = 0;
	if (retval = parser_read(p))
		goto fail;


	vsm_start(v, 0);

  fail:
	vsm_free(v);

	return retval;
}
