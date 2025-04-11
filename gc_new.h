#pragma once

#include "gc_object.h"
#include "gc_vm.h"

gc_object_t *new_gc_integer(gc_vm_t *vm, int value);
gc_object_t *new_gc_float(gc_vm_t *vm, float value);
gc_object_t *new_gc_string(gc_vm_t *vm, char *value);
gc_object_t *new_gc_vector3(gc_vm_t *vm, gc_object_t *x, gc_object_t *y,
                            gc_object_t *z);
gc_object_t *new_gc_array(gc_vm_t *vm, size_t size);

gc_object_t *gc_add(gc_vm_t *vm, gc_object_t *a, gc_object_t *b);
