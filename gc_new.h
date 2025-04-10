#include "gc_object.h"


gc_object_t *new_gc_integer(int value);
gc_object_t *new_gc_float(float value);
gc_object_t *new_gc_string(char *value);
gc_object_t *new_gc_vector3(gc_object_t *x, gc_object_t *y, gc_object_t *z);
gc_object_t *new_gc_array(size_t size);


gc_object_t *gc_add(gc_object_t *a, gc_object_t *b);