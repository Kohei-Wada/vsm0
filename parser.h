#ifndef _PARSER_H_
#define _PARSER_H_


typedef struct parser parser_t;

int parser_init(parser_t **p);
void parser_free(parser_t *p);

#endif
