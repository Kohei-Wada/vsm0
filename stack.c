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


int stack_get_sp(stack_t *s)
{
	return s->sp;
}


void stack_set_sp(stack_t *s, int val)
{
	s->sp = val;
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


