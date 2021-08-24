#ifndef _STACK_H_
#define _STACK_H_

#define STACK_SIZE 100

typedef struct stack stack_t;
typedef int* sp_t;



int stack_init(stack_t **s);
void stack_free(stack_t *s);



#endif
