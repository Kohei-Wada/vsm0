#ifndef _NMTABLE_H_
#define _NMTABLE_H_



typedef struct nmtable nmtable_t;

int nmtable_init(nmtable_t **p);
void nmtable_free(nmtable_t *p);

#endif
