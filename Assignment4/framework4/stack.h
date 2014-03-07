/*
 * This is the interface for a generic stack.
 */

#ifndef _STACK_H
#define _STACK_H

#define STACK_SUCCESS 0
#define STACK_ERROR -1

#define STACK_EMPTY -2
#define STACK_FULL -3

typedef struct stack_t {
	void **data;
	int max_size;
	int top;
} stack_t;

/* Returns an initialized stack object, else NULL upon failure. */
stack_t* stack_init(int max_size);

/* Frees the stack object, but NOT the elements pointed to by data! */
void stack_destroy(stack_t *stack);

/* Returns STACK_SUCCES upon success, else a STACK_something error value. */
int stack_push(stack_t *stack, void *element);

/* Popping an element does NOT free any memory! Returns NULL if empty stack. */
void* stack_pop(stack_t *stack);

/* Returns 0 if False, 1 if True */
int stack_is_empty(stack_t *stack);
int stack_is_full(stack_t *stack);

#endif
