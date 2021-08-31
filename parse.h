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

char *parser_id_entry(parser_t *p, char *name, int len);

int parser_sym_decl(parser_t *p, char *name);
int parser_sym_ref(parser_t *p, char *name);

int parser_cout(parser_t *p, op_t op, int i);
#endif
