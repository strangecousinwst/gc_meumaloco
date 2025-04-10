#include <stdbool.h>
#include <stddef.h>
#include <typeindex>

typedef struct GC_Object gc_object_t;

typedef struct {
  size_t size;
  gc_object_t **elements;
} gc_array_t;

typedef struct {
  gc_object_t *x;
  gc_object_t *y;
  gc_object_t *z;
} gc_vector_t;

typedef enum GC_Object_Type {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,
} gc_object_type_t;

typedef union GC_Object_Data {
  int v_int;
  float v_float;
  char *v_string;
  gc_vector_t v_vector3;
  gc_array_t v_array;
} gc_object_data_t;

typedef struct GC_Object {
  int ref_count;
  gc_object_type_t type;
  gc_object_data_t data;
} gc_object_t;

gc_object_t *new_gc_integer(int value);
gc_object_t *new_gc_float(float value);
gc_object_t *new_gc_string(char *value);
gc_object_t *new_gc_vector3(gc_object_t *x, gc_object_t *y, gc_object_t *z);
gc_object_t *new_gc_array(size_t size);

void ref_count_inc(gc_object_t *obj);
void ref_count_dec(gc_object_t *obj);
void ref_count_free(gc_object_t *obj);

bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value);
