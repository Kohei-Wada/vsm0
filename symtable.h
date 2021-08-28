#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_


typedef struct symtable symtable_t;

int symtable_init(symtable_t **p);
void symtable_free(symtable_t *p);

#endif
