#include <stdlib.h> 
#include "nmtable.h"

typedef struct nmtable{
	id_entry_t *id_entry[NAMETABLE_SIZE];
} nmtable_t;


static int id_entry_init(id_entry_t **e, char *name, int len, id_entry_t *next)
{
	*e = malloc(sizeof(id_entry_t));
	if (!(*e))
		return 1;

	(*e)->name = name;
	(*e)->len  = len;
	(*e)->next = next;

	return 0;
}


static void id_entry_free(id_entry_t *e)
{
	free(e);
}


int nmtable_init(nmtable_t **n)
{
	*n = malloc(sizeof(nmtable_t));
	if (!(*n))
		return 1;

	for (int i = 0; i < NAMETABLE_SIZE; ++i)
		(*n)->id_entry[i] = NULL;

	return 0;
}


void nmtable_free(nmtable_t *n)
{
	free(n);
}


static int nmtable_hash(nmtable_t *n, char *sp)
{
	unsigned int h, g;

	for (h = 0; *sp != '\0'; sp++) {
		h = (h << 4) + (unsigned)(*sp);

		if (g = h & 0xf0000000)
			h = (h ^ g >> 24) ^ g;
	}

	return h %  NAMETABLE_SIZE;
}



int nmtable_add(nmtable_t *n, char *id_name, int len)
{
	return 0;
}




