#ifndef _PARSER_H_
#define _PARSER_H_

#include "vsm.h"

typedef struct parser parser_t;

int parser_init(parser_t **p, vsm_t *v);
void parser_free(parser_t *p);

#endif
