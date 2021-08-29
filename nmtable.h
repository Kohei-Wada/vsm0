#ifndef NMTABLE_H
#define NMTABLE_H


#define NMTABLE_SIZE 100

typedef struct id_entry {
	char *name;
	int len;
	struct id_entry *next;
} id_entry_t;

typedef struct nmtable nmtable_t;

int nmtable_init(nmtable_t **p);
void nmtable_free(nmtable_t *p);

char* nmtable_add(nmtable_t *n, char *id_name, int len);
#endif
