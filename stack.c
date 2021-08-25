#include <stdlib.h>
#include <stdio.h>
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


static int stack_get_array_val(stack_t *s, int sp)
{
	return  s->array[sp];
}


static int stack_get_stack_size(stack_t *s)
{
	return s->stack_size;
}


static void stack_set_array_val(stack_t *s, int sp, int val)
{
	s->array[sp] = val;
}


int stack_pop(stack_t *s)
{
	int sp = stack_get_sp(s);

	if (sp == 0) {
		/*TODO handle error.*/
		fprintf(stderr, "failed to pop value\n");
	}

	stack_set_sp(s, sp - 1);
	return stack_get_array_val(s, sp);
}


int stack_push(stack_t *s, int val)
{
	int sp = stack_get_sp(s) + 1;

	if (sp > stack_get_stack_size(s)) {
		fprintf(stderr, "failed to push value\n");
		return 1;
	}

	stack_set_sp(s, sp);
	stack_set_array_val(s, sp, val); 

	return 0;
}


int stack_init(stack_t **s)
{
	*s = malloc(sizeof(stack_t));

	if (!(*s)) {
		perror("malloc");
		return 1;
	}

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

