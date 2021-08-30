#ifndef PARSER_H
#define PARSER_H

typedef struct vsm vsm_t;
typedef enum op op_t;
typedef struct parser parser_t;

int parser_init(parser_t **p, vsm_t *v);
void parser_free(parser_t *p);
int parser_read(parser_t *p);

void parser_handle_simple_op(parser_t *p, op_t op);
void parser_handle_num(parser_t *p, int num);

#endif
