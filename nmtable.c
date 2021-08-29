#include <stdlib.h> 
#include <string.h>

#include "nmtable.h"

typedef struct nmtable{
	id_entry_t *table[NMTABLE_SIZE];
} nmtable_t;

/***********************************************************************************/

static int id_entry_init(id_entry_t **e, char *name, int len, id_entry_t *next)
{
	*e = malloc(sizeof(id_entry_t));
	if (!(*e))
		return 1;

	strcpy((*e)->name, name);
	(*e)->len  = len;
	(*e)->next = next;

	return 0;
}


static void id_entry_free(id_entry_t *e)
{
	free(e);
}


static char* id_entry_get_name(id_entry_t *e)
{
	return e->name;
}


static int id_entry_get_len(id_entry_t *e)
{
	return e->len;
}


static id_entry_t* id_entry_get_next(id_entry_t *e)
{
	return e->next;
}


/***********************************************************************************/


id_entry_t **nmtable_get_table(nmtable_t *n)
{
	return n->table;
}


int nmtable_init(nmtable_t **n)
{
	*n = malloc(sizeof(nmtable_t));
	if (!(*n))
		return 1;

	for (int i = 0; i < NMTABLE_SIZE; ++i)
		(*n)->table[i] = NULL;

	return 0;
}


void nmtable_free(nmtable_t *n)
{
	id_entry_t **table = nmtable_get_table(n);

	for (int i = 0; i < NMTABLE_SIZE; ++i)
		id_entry_free(table[i]);

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
	return h %  NMTABLE_SIZE;
}


static id_entry_t *nmtable_search(nmtable_t *n, char *sp, int len, int hash)
{
	id_entry_t **table = nmtable_get_table(n);
	id_entry_t *entry;

	for (entry =table[hash]; entry != NULL; entry = id_entry_get_next(entry)) {
		if ((id_entry_get_len(entry) == len && strcmp(id_entry_get_name(entry), sp) == 0))
			return entry;
	}

	return NULL;
}


char* nmtable_add(nmtable_t *n, char *id_name, int len)
{
	id_entry_t *entry, *new;
	id_entry_t **table = nmtable_get_table(n);

	int hash  = nmtable_hash(n, id_name);

	if (entry = nmtable_search(n, id_name, len, hash))
		return id_entry_get_name(entry);

	id_entry_init(&new, id_name, len, table[hash]);

	table[hash] = new;

	return NULL;
}	


