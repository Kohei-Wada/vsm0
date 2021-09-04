#ifndef PARSER_H
#define PARSER_H

typedef struct vsm vsm_t;
typedef enum op op_t;
typedef struct parser parser_t;

int parser_init(parser_t **p, vsm_t *v);
void parser_free(parser_t *p);
int parser_read(parser_t *p);

int parser_set_input_file(parser_t *p, const char *file_name);

void parser_handle_simple_op(parser_t *p, op_t op);
void parser_handle_num(parser_t *p, int num);
void parser_handle_ppmm(parser_t *p, op_t op, char *ppmm, int priorize);
void parser_handle_id(parser_t *p, op_t op, char *id_name);
void parser_handle_relop(parser_t *p, op_t op);


int parser_sym_decl(parser_t *p, char *name, int init_value);
int parser_sym_ref(parser_t *p, char *name);
char *parser_id_entry(parser_t *p, char *name, int len);
#endif
