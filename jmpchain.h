#ifndef JMPCHAIN
#define JMPCHAIN

#define JMPCHAIN_SIZE 20
typedef struct parser parser_t;

typedef struct jmpchain jmpchain_t;

int jmpchain_init(jmpchain_t **j, parser_t *p);
void jmpchain_free(jmpchain_t *j);

int jmpchain_get_csptr(jmpchain_t *j);
void jmpchain_nestin(jmpchain_t *j, int st);
void jmpchain_nestout(jmpchain_t *j, int contp);
void jmpchain_break(jmpchain_t *j, int jc);
void jmpchain_conti(jmpchain_t *j);

#endif
