#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 100

typedef struct stack stack_t;
typedef int* sp_t;

int stack_pop(stack_t *s);
int stack_push(stack_t *s, int val);

void stack_set_sp(stack_t *s, int val);
int stack_get_sp(stack_t *s);

void stack_set_max_sp(stack_t *s, int count);
int stack_get_max_sp(stack_t *s);

int stack_init(stack_t **s);
void stack_free(stack_t *s);

#endif
