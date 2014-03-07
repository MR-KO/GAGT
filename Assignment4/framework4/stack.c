/*
 * This file implements a stack defined in stack.h and uses an array of
 * void pointers of size max_size. top is an index for the array pointing to
 * top of the array
 */

#include <stdio.h>
#include <stdlib.h>  /* for dynamic allocation */
#include "stack.h"

/************************ Function Definitions **********************/

stack_t* stack_init(int max_size) {
	/* Allocate a new stack object */
	stack_t *stack = malloc(sizeof(stack_t));

	if (stack == NULL) {
		return NULL;
	}

	/* Allocate a new array to hold the data pointers. */
	stack->data = calloc(max_size, sizeof(void*));

	if (stack->data == NULL) {
		stack_destroy(stack);
		return NULL;
	}

	stack->max_size = max_size;
	stack->top = STACK_EMPTY;
	return stack;
}

void stack_destroy(stack_t *stack) {
	if (stack != NULL) {
		// free(stack->data);qq
		free(stack);
		stack = NULL;
	}
}

int stack_push(stack_t *stack, void *element) {
	if (stack == NULL) {
		return STACK_ERROR;
	}

	if (stack_is_full(stack)) {
		return STACK_FULL;
	}

	stack->data[++(stack->top)] = element;
	return STACK_SUCCESS;
}

void* stack_pop(stack_t *stack) {
	if (stack == NULL) {
		return NULL;
	}

	if (stack_is_empty(stack)) {
		return NULL;
	}

	return stack->data[stack->top--];
}

int stack_is_empty(stack_t *stack) {
	if (stack != NULL) {
		return stack->top < 0;
	} else {
		return STACK_ERROR;
	}
}

int stack_is_full(stack_t *stack) {
	if (stack != NULL) {
		return stack->top >= stack->max_size - 1;
	} else {
		return STACK_ERROR;
	}
}
