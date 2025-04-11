#pragma once

#include "gc_stack.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct GC_Object gc_object_t;

int gc_length(gc_object_t *gc_obj);

typedef struct GC_Array {
  size_t size;
  gc_object_t **elements;
} gc_array_t;

typedef struct GC_Vector {
  gc_object_t *x;
  gc_object_t *y;
  gc_object_t *z;
} gc_vector_t;

typedef enum GC_Object_Type {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY
} gc_object_type_t;

typedef union GC_Object_Data {
  int v_int;
  float v_float;
  char *v_string;
  gc_vector_t v_vector3;
  gc_array_t v_array;
} gc_object_data_t;

typedef struct GC_Object {
  bool is_marked;
  gc_object_type_t type;
  gc_object_data_t data;
} gc_object_t;

void gc_object_free(gc_object_t *obj);

bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value);
gc_object_t *gc_array_get(gc_object_t *gc_obj, size_t index);

int gc_length(gc_object_t *obj);

void ref_count_inc(gc_object_t *obj);
void ref_count_dec(gc_object_t *obj);
void ref_count_free(gc_object_t *obj);