#include <stdlib.h>
#include <stdio.h>

#include "stack.h"


typedef struct stack {
	int sp;
	int stack_size;
	int *array;
	int max_sp;
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

static int stack_set_stack_size(stack_t *s, int size)
{
	s->stack_size = size;
}


static void stack_set_array_val(stack_t *s, int sp, int val)
{
	s->array[sp] = val;
}


void stack_set_max_sp(stack_t *s, int count)
{
	s->max_sp = count;
}


int stack_get_max_sp(stack_t *s)
{
	return s->max_sp;
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

	if (!(*s)) 
		goto error;

	(*s)->array = malloc(sizeof(int) * STACK_SIZE);

	if (!(*s)->array)
		goto error;

	stack_set_sp(*s, 0);
	stack_set_max_sp(*s, 0);
	stack_set_stack_size(*s, STACK_SIZE);

	return 0;


  error:
	perror("malloc");
	return 1;
}


void stack_free(stack_t *s)
{
	free(s->array);
	free(s);
}

