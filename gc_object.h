#pragma once

#include "gc_stack.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct GC_Object gc_object_t;

int gc_length(gc_object_t *gc_obj);

// Estrutura de um array
typedef struct GC_Array {
  size_t size;
  gc_object_t **elements;
} gc_array_t;

// Estrutura de um vetor de 3 dimensoes
typedef struct GC_Vector {
  gc_object_t *x;
  gc_object_t *y;
  gc_object_t *z;
} gc_vector_t;

// Enums dos tipos que um objeto pode ser
typedef enum GC_Object_Type {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY
} gc_object_type_t;

// O tipo de dados que um objeto pode ter
typedef union GC_Object_Data {
  int v_int;
  float v_float;
  char *v_string;
  gc_vector_t v_vector3;
  gc_array_t v_array;
} gc_object_data_t;

// Estrutura de um objeto do garbage collector
typedef struct GC_Object {
  bool is_marked;
  gc_object_type_t type;
  gc_object_data_t data;
} gc_object_t;

// Funçao para libertar um objeto
void gc_object_free(gc_object_t *obj);

// Get/Set de um array
bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value);
gc_object_t *gc_array_get(gc_object_t *gc_obj, size_t index);

// Funçao para sacar o comprimento de um objeto
int gc_length(gc_object_t *obj);

// void ref_count_inc(gc_object_t *obj);
// void ref_count_dec(gc_object_t *obj);
// void ref_count_free(gc_object_t *obj);
