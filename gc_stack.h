#pragma once

#include <stddef.h>
#include <stdlib.h>

typedef struct GC_stack {
  size_t count;
  size_t capacity;
  void **data;
} gc_stack_t;

gc_stack_t *gc_stack_new(size_t capacity);

void gc_stack_push(gc_stack_t *stack, void *obj);
void *gc_stack_pop(gc_stack_t *stack);

void gc_stack_free(gc_stack_t *stack);
void gc_stack_remove_nulls(gc_stack_t *stack);
