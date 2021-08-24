#include <stdlib.h>
#include "stack.h"


typedef struct stack {
	int sp;
	int stack_size;
	int *array;
} stack_t;


int stack_pop(stack_t *s)
{
	int *stack = s->array;
	return 0;
}


int stack_push(stack_t *s)
{
	int *stack = s->array;
	return 0;
}


int stack_init(stack_t **s, sp_t **sp)
{
	*s = malloc(sizeof(stack_t));
	*sp = malloc(sizeof(sp_t));

	if (!(*s) || !(*sp)) 
		return 1;

	(*s)->sp = 0;
	(*s)->stack_size = STACK_SIZE;
	(*s)->array = malloc(sizeof(int) * STACK_SIZE);

	*sp = 0;

	return 0;
}


void stack_free(stack_t *s)
{
	free(s->array);
	free(s);
}


