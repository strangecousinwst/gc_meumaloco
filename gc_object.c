#include <assert.h>
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


gc_object_t *gc_add(gc_object_t *a, gc_object_t *b) {
  if (a == NULL || b == NULL) { return NULL; }

  switch (a->type) {
    case INTEGER: {
      switch (b->type) {
        case INTEGER: return new_gc_integer(a->data.v_int + b->data.v_int);
        case FLOAT: return new_gc_float((float)a->data.v_int + b->data.v_float);
        default: return NULL;
      }
    }
    case FLOAT: {
      switch (b->type) {
        case INTEGER: return new_gc_float(a->data.v_float + (float)b->data.v_int);
        case FLOAT: return new_gc_float(a->data.v_float + b->data.v_float);
        default: return NULL;
      }
    }
    case STRING: {
      if (b->type != STRING) { return NULL; }
      size_t len = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
      char *str = calloc(sizeof(char), len);
      strcat(str, a->data.v_string);
      strcat(str, b->data.v_string);
      gc_object_t *obj = new_gc_string(str);
      free(str);
      return obj;
    }
    case VECTOR3: {
      if (b->type != VECTOR3) { return NULL; }
      return new_gc_vector3(
        gc_add(a->data.v_vector3.x, b->data.v_vector3.x),
        gc_add(a->data.v_vector3.y, b->data.v_vector3.y),
        gc_add(a->data.v_vector3.z, b->data.v_vector3.z)
      );
    }
    case ARRAY: {
      if (b->type != ARRAY) { return NULL; }
      size_t len = a->data.v_array.size + b->data.v_array.size;
      gc_object_t *obj = new_gc_array(len);
      
      for (size_t i = 0; i < a->data.v_array.size; i++){
        gc_array_set(obj, i, gc_array_get(a, i));
      }

      for (size_t i = 0; i < b->data.v_array.size; i++){
        gc_array_set(obj, i + a->data.v_array.size, gc_array_get(b, i));
      }
      
      return obj;
    }
    default: return NULL;
  }
}

int gc_length(gc_object_t *obj) {
  if (obj == NULL) { return -1; }

  switch (obj->type) {
    case INTEGER: return 1;
    case FLOAT: return 1;
    case STRING: return strlen(obj->data.v_string);
    case VECTOR3: return 3;
    case ARRAY: return obj->data.v_array.size;
    default: return -1;
  }
}

bool gc_array_set(gc_object_t *gc_obj, size_t index, gc_object_t *value) {
  if (gc_obj == NULL || value == NULL) { return false;}
  if (gc_obj->type != ARRAY) { return false; }
  if (gc_obj->data.v_array.size <= index) { return false;}

  gc_obj->data.v_array.elements[index] = value;

  return true;
}

gc_object_t *gc_array_get(gc_object_t *gc_obj, size_t index) {
  if (gc_obj == NULL) { return NULL;}
  if (gc_obj->type != ARRAY) { return NULL; }
  if (gc_obj->data.v_array.size <= index) { return NULL;}


  return gc_obj->data.v_array.elements[index];
}

gc_object_t *new_gc_array(size_t size) {
  gc_object_t *obj = _new_gc_object();
  if (obj == NULL) { return NULL; }

  gc_object_t **elements = calloc(size, sizeof(gc_object_t *));
  if (elements == NULL) { return NULL; }

  obj->type = ARRAY;
  obj->data.v_array = (gc_array_t){.size = size, .elements = elements};

  return obj;
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

gc_object_t *new_gc_string(char *value) {
  gc_object_t *obj = _new_gc_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->type = STRING;
  obj->data.v_string = malloc(strlen(value) + 1);
  if (obj->data.v_string == NULL) {
    return NULL;
  }
  strcpy(obj->data.v_string, value);

  return obj;
}

gc_object_t *new_gc_float(float value) {
  gc_object_t *obj = _new_gc_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->type = FLOAT;
  obj->data.v_float = value;

  return obj;
}

gc_object_t *new_gc_integer(int value) {
  gc_object_t *obj = _new_gc_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->type = INTEGER;
  obj->data.v_int = value;

  return obj;
}