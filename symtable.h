#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_


#define SYMTABLE_SIZE 100

typedef struct symtable symtable_t;

int symtable_init(symtable_t **p);
void symtable_free(symtable_t *p);
int symtable_decl(symtable_t *s, char *symname);

#endif
