#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "y.tab.h"
#include "vsm.h"
#include "test.c"

int main(void) 
{

	vsm_t *v;
	vsm_init(&v);
//	vsm_set_trace(v, 1);

	set_yyvsm(v);
	yyparse();

	vsm_start(v, 0, 0);

	vsm_free(v);

	return 0;
}
