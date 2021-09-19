#include <stdlib.h>

#include "label.h"


typedef struct case_entry {
	int label;
	int addr;
} centry_t;


typedef struct {
	int swtptr;
	centry_t array[SWT_SIZE];
