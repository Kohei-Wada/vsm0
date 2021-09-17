#ifndef PARSER_H
#define PARSER_H

typedef struct vsm vsm_t;
typedef enum op op_t;
typedef struct parser parser_t;
typedef struct jmpchain jmpchain_t;

int parser_init(parser_t **p, vsm_t *v);
void parser_free(parser_t *p);
int parser_read(parser_t *p);

int parser_set_input_file(parser_t *p, const char *file_name);

void parser_handle_id(parser_t *p, op_t op, char *id_name);
void parser_handle_relop(parser_t *p, op_t op);


vsm_t *parser_get_vsm(parser_t *p);
int parser_get_pc(parser_t *p);
void parser_inc_pc(parser_t *p);

jmpchain_t *parser_get_jchain(parser_t *p);


void parser_set_instr(parser_t *p, int pc, op_t op, int flag, int addr);
int parser_back_patching(parser_t *p, int loc, int target);

void parser_set_pc(parser_t *p, int value);
int parser_sym_decl(parser_t *p, char *name, int init_value);
int parser_sym_ref(parser_t *p, char *name);
char *parser_id_entry(parser_t *p, char *name, int len);
#endif
