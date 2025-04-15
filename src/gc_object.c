#pragma once

#include "gc_object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gc_object_free(gc_object_t *obj) {
  switch (obj->type) {
  case INTEGER:
    break;
  case FLOAT:
    break;
  case STRING: {
    free(obj->data.v_string);
    break;
  }
  case VECTOR3:
    break;
  case ARRAY: {
    free(obj->data.v_array.elements);
    break;
  }
  }
  free(obj);
}

int gc_length(gc_object_t *obj) {
  if (obj == NULL) {
    return -1;
  }

  switch (obj->type) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    return -1;
  }
}

bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value) {
  if (gc_obj == NULL || value == NULL) {
    return false;
  }
  if (gc_obj->type != ARRAY) {
    return false;
  }
  if (gc_obj->data.v_array.size <= index) {
    return false;
  }

  gc_obj->data.v_array.elements[index] = value;

  return true;
}

gc_object_t *gc_array_get(gc_object_t *gc_obj, size_t index) {
  if (gc_obj == NULL) {
    return NULL;
  }
  if (gc_obj->type != ARRAY) {
    return NULL;
  }
  if (gc_obj->data.v_array.size <= index) {
    return NULL;
  }

  return gc_obj->data.v_array.elements[index];
}
