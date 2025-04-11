#pragma once

#include "gc_new.h"
#include "gc_object.h"
#include "gc_vm.h"

#include <stdlib.h>
#include <string.h>

gc_object_t *_new_gc_object(gc_vm_t *vm) {
  gc_object_t *obj = calloc(1, sizeof(gc_object_t));
  if (obj == NULL) {
    return NULL;
  }

  // tracking do objeto
  gc_vm_track_object(vm, obj);
  //
  // Inicializar como nao marcado
  obj->is_marked = false;

  return obj;
}

gc_object_t *gc_add(gc_vm_t *vm, gc_object_t *a, gc_object_t *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  // Casos particulares da adicionar 2 objetos
  switch (a->type) {
    // Caso a seja integer
  case INTEGER: {
    switch (b->type) {
      // E b seja integer tambem
    case INTEGER:
      return new_gc_integer(vm, a->data.v_int + b->data.v_int);
      // E b seja float
    case FLOAT:
      return new_gc_float(vm, (float)a->data.v_int + b->data.v_float);
      // Se b nao for integer ou float return null
    default:
      return NULL;
    }
  }
    // Caso a seja float
  case FLOAT: {
    switch (b->type) {
      // E b seja integer
    case INTEGER:
      return new_gc_float(vm, a->data.v_float + (float)b->data.v_int);
      // E b seja float
    case FLOAT:
      return new_gc_float(vm, a->data.v_float + b->data.v_float);
      // Se b nao for integer ou float return null
    default:
      return NULL;
    }
  }
    // Caso a seja string
  case STRING: {
    // Se b nao for string tambem return null
    if (b->type != STRING) {
      return NULL;
    }
    size_t len = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
    char *str = calloc(sizeof(char), len);
    strcat(str, a->data.v_string);
    strcat(str, b->data.v_string);
    gc_object_t *obj = new_gc_string(vm, str);
    free(str);
    return obj;
  }
    // Caso a seja vector3
  case VECTOR3: {
    // Se b nao for vector3 tambem return null
    if (b->type != VECTOR3) {
      return NULL;
    }
    return new_gc_vector3(vm,
                          gc_add(vm, a->data.v_vector3.x, b->data.v_vector3.x),
                          gc_add(vm, a->data.v_vector3.y, b->data.v_vector3.y),
                          gc_add(vm, a->data.v_vector3.z, b->data.v_vector3.z));
  }
    // Caso a seja array
  case ARRAY: {
    // Se b nao for array tambem return null
    if (b->type != ARRAY) {
      return NULL;
    }
    size_t len = a->data.v_array.size + b->data.v_array.size;
    gc_object_t *obj = new_gc_array(vm, len);

    for (size_t i = 0; i < a->data.v_array.size; i++) {
      gc_array_set(obj, i, gc_array_get(a, i));
    }

    for (size_t i = 0; i < b->data.v_array.size; i++) {
      gc_array_set(obj, i + a->data.v_array.size, gc_array_get(b, i));
    }

    return obj;
  }
  default:
    return NULL;
  }
}

gc_object_t *new_gc_array(gc_vm_t *vm, size_t size) {
  gc_object_t *obj = _new_gc_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  gc_object_t **elements = calloc(size, sizeof(gc_object_t *));
  if (elements == NULL) {
    return NULL;
  }

  obj->type = ARRAY;
  obj->data.v_array = (gc_array_t){.size = size, .elements = elements};

  gc_vm_track_object(vm, obj);
  return obj;
}

gc_object_t *new_gc_vector3(gc_vm_t *vm, gc_object_t *x, gc_object_t *y,
                            gc_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  gc_object_t *obj = _new_gc_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->type = VECTOR3;
  obj->data.v_vector3 = (gc_vector_t){.x = x, .y = y, .z = z};

  gc_vm_track_object(vm, obj);
  return obj;
}

gc_object_t *new_gc_string(gc_vm_t *vm, char *value) {
  gc_object_t *obj = _new_gc_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->type = STRING;
  obj->data.v_string = malloc(strlen(value) + 1);
  if (obj->data.v_string == NULL) {
    return NULL;
  }
  strcpy(obj->data.v_string, value);

  gc_vm_track_object(vm, obj);
  return obj;
}

gc_object_t *new_gc_float(gc_vm_t *vm, float value) {
  gc_object_t *obj = _new_gc_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->type = FLOAT;
  obj->data.v_float = value;

  gc_vm_track_object(vm, obj);
  return obj;
}

gc_object_t *new_gc_integer(gc_vm_t *vm, int value) {
  gc_object_t *obj = _new_gc_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->type = INTEGER;
  obj->data.v_int = value;

  gc_vm_track_object(vm, obj);
  return obj;
}
