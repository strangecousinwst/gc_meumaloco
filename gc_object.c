#include <assert.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gc_object.h"

gc_object_t *_new_gc_object() {
  gc_object_t *obj = malloc(sizeof(gc_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->ref_count = 1;
  return obj;
}

void ref_count_dec(gc_object_t *obj) {
  if (obj == NULL) {
    return;
  }

  obj->ref_count--;
  if (obj->ref_count == 0) {
    ref_count_free(obj);
  }
}

void ref_count_free(gc_object_t *obj) {
  if (obj == NULL) {
    return;
  }

  switch (obj->type) {
  case INTEGER:
    break;
  case FLOAT:
    break;
  case STRING: {
    free(obj->data.v_string);
    break;
  }
  case VECTOR3: {
    ref_count_dec(obj->data.v_vector3.x);
    ref_count_dec(obj->data.v_vector3.y);
    ref_count_dec(obj->data.v_vector3.z);
    break;
  }
  case ARRAY: {
    gc_array_t arr = obj->data.v_array;
    for (int i = 0; i < arr.size; i++) {
      ref_count_dec(arr.elements[i]);
    }
    free(obj->data.v_array.elements);
    break;
  }
  default: {
    assert(false);
  }
    free(obj);
  }
}

void ref_count_inc(gc_object_t *obj) {
  if (obj == NULL) {
    return;
  }

  obj->ref_count++;
}

bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value) {
  if (gc_obj == NULL || value == NULL) {
    return false;
  }
  if (gc_obj->type != ARRAY) {
    return false;
  }
  if (index >= gc_obj->data.v_array.size) {
    return false;
  }

  ref_count_inc(value);
  if (gc_obj->data.v_array.elements[index] != NULL) {
    ref_count_dec(gc_obj->data.v_array.elements[index]);
  }

  gc_obj->data.v_array.elements[index] = value;
  return true;
}

gc_object_t *new_gc_vector3(gc_object_t *x, gc_object_t *y, gc_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  gc_object_t *obj = _new_gc_object();

  if (obj == NULL) {
    return NULL;
  }

  obj->type = VECTOR3;
  ref_count_inc(x);
  ref_count_inc(y);
  ref_count_inc(z);
  obj->data.v_vector3 = (gc_vector_t){.x = x, .y = y, .z = z};
  return obj;
}

gc_object_t *new_gc_array(size_t size) {
  gc_object_t *obj = _new_gc_object();
  if (obj == NULL) {
    return NULL;
  }

  gc_object_t **elements = calloc(size, sizeof(gc_object_t *));
  if (elements == NULL) {
    free(obj);
    return NULL;
  }

  obj->type = ARRAY;
  obj->data.v_array = (gc_array_t){.size = size, .elements = elements};

  return obj;
}
