#include "gc_object.h"
#include <stddef.h>

typedef struct GC_stack {
  size_t count;
  size_t size;
  gc_object_t **elements;
} gc_stack_t;

gc_stack_t *stack_new(size_t capacity);
void stack_push(size_t size);
