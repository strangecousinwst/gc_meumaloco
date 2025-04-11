#include "gc_stack.h"

#include <stddef.h>
#include <stdlib.h>

void gc_stack_free(gc_stack_t *stack) {
  if (stack == NULL) { return; }
  if (stack->data != NULL) { free(stack->data); }

  free(stack);
}

void *gc_stack_pop(gc_stack_t *stack) {
  if (stack->count == 0) { return NULL; }
  stack->count--;
  return stack->data[stack->count];
}

void gc_stack_push(gc_stack_t *stack, void *obj) {
  if (stack->count == stack->capacity) {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, sizeof(void *) * stack->capacity);
    if (stack->data == NULL) { return; }
  }

  stack->data[stack->count] = obj;
  stack->count++;
}

gc_stack_t *gc_stack_new(size_t capacity) {
  gc_stack_t *stack = malloc(sizeof(gc_stack_t));
  if (stack == NULL) { return NULL; }

  stack->count = 0;
  stack->capacity = capacity;
  stack->data = malloc(sizeof(void *) * capacity);
  if (stack->data == NULL) { free(stack); }

  return stack;
}

void gc_stack_remove_nulls(gc_stack_t *stack) {
  size_t new_count = 0;

  for (size_t i = 0; i < stack->count; i++) {
    if (stack->data[i] != NULL) {
      stack->data[new_count++] = stack->data[i];
    }
  }
  stack->count = new_count;

  for (size_t i = new_count; i < stack->capacity; i++) {
    stack->data[i] = NULL;
  }
}
