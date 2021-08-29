#ifndef PARSER_H
#define PARSER_H

typedef struct vsm vsm_t;

typedef struct parser parser_t;

int parser_init(parser_t **p, vsm_t *v);
void parser_free(parser_t *p);
void parser_read(parser_t *p);

#endif
