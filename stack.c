#include <stdlib.h>
#include "stack.h"


typedef struct stack {
	int sp;
	int stack_size;
	int *array;
} stack_t;


int stack_get_sp(stack_t *s)
{
	return s->sp;
}


void stack_set_sp(stack_t *s, int val)
{
	s->sp = val;
}


int stack_pop(stack_t *s)
{
	int sp = stack_get_sp(s);
	int retval = s->array[sp];
	stack_set_sp(s, sp - 1);

	return retval;
}


int stack_push(stack_t *s, int val)
{
	int sp = stack_get_sp(s);
	s->array[sp + 1] = val;
	stack_set_sp(s, sp + 1);

	return 0;
}


int stack_init(stack_t **s)
{
	*s = malloc(sizeof(stack_t));

	if (!(*s)) 
		return 1;

	(*s)->sp = 0;
	(*s)->stack_size = STACK_SIZE;
	(*s)->array = malloc(sizeof(int) * STACK_SIZE);

	return 0;
}


void stack_free(stack_t *s)
{
	free(s->array);
	free(s);
}

