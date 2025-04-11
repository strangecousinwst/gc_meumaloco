#pragma once

#include "gc_vm.h"
#include "gc_object.h"
#include "gc_stack.h"

void gc_collect_garbage(gc_vm_t *vm) {
  gc_mark(vm);
  gc_trace(vm);
  gc_sweep(vm);
}

void gc_sweep(gc_vm_t *vm) {
  for (int i = 0; i < vm->objects->count; i++) {
    gc_object_t *obj = vm->objects->data[i];
    if (obj->is_marked) {
      obj->is_marked = false;
    } else {
      gc_object_free(obj);
      vm->objects->data[i] = NULL;
    }
  }

  gc_stack_remove_nulls(vm->objects);
}

void gc_trace(gc_vm_t *vm) {
  if (vm == NULL) {
    return;
  }

  gc_stack_t *gray_objects = gc_stack_new(8);
  if (gray_objects == NULL) {
    return;
  }

  for (int i = 0; i < vm->objects->count; i++) {
    gc_object_t *obj = vm->objects->data[i];
    if (obj && obj->is_marked) {
      gc_stack_push(gray_objects, obj);
    }
  }

  while (gray_objects->count > 0) {
    gc_object_t *obj = gc_stack_pop(gray_objects);
    gc_trace_blacken_object(gray_objects, obj);
  }

  gc_stack_free(gray_objects);
}

void gc_trace_blacken_object(gc_stack_t *gray_objects, gc_object_t *obj) {
  if (obj == NULL) {
    return;
  }

  switch (obj->type) {
  case INTEGER:
    return;
  case FLOAT:
    return;
  case STRING:
    return;
  case VECTOR3: {
    gc_trace_blacken_object(gray_objects, obj->data.v_vector3.x);
    gc_trace_blacken_object(gray_objects, obj->data.v_vector3.y);
    gc_trace_blacken_object(gray_objects, obj->data.v_vector3.z);
    break;
  }
  case ARRAY: {
    gc_array_t arr = obj->data.v_array;
    for (int i = 0; i < arr.size; i++) {
      gc_trace_mark_object(gray_objects, gc_array_get(obj, i));
    }
    break;
  }
  }
}

void gc_trace_mark_object(gc_stack_t *gray_objects, gc_object_t *obj) {
  if (obj == NULL) {
    return;
  }
  if (obj->is_marked) {
    return;
  }

  obj->is_marked = true;
  gc_stack_push(gray_objects, (void *)obj);
}

void gc_mark(gc_vm_t *vm) {
  if (vm == NULL) {
    return;
  }

  for (int i = 0; i < vm->frames->count; i++) {
    gc_frame_t *frame = vm->frames->data[i];
    for (int j = 0; j < frame->references->count; j++) {
      gc_object_t *obj = frame->references->data[j];
      obj->is_marked = true;
    }
  }
}

void gc_vm_frame_reference_object(gc_frame_t *frame, gc_object_t *obj) {
  if (frame == NULL || obj == NULL) {
    return;
  }
  gc_stack_push(frame->references, (void *)obj);
}

void gc_vm_push_frame(gc_vm_t *vm, gc_frame_t *frame) {
  if (vm == NULL || frame == NULL) {
    return;
  }
  gc_stack_push(vm->frames, (void *)frame);
}

gc_frame_t *gc_vm_new_frame(gc_vm_t *vm) {
  gc_frame_t *frame = malloc(sizeof(gc_frame_t));
  if (frame == NULL) {
    return NULL;
  }

  frame->references = gc_stack_new(8);
  if (frame->references == NULL) {
    free(frame);
    return NULL;
  }

  gc_vm_push_frame(vm, frame);
  return frame;
}

void gc_vm_free_frame(gc_frame_t *frame) {
  if (frame == NULL) {
    return;
  }

  gc_stack_free(frame->references);
  free(frame);
}

void gc_vm_free(gc_vm_t *vm) {
  if (vm == NULL) {
    return;
  }

  for (int i = 0; i < vm->frames->count; i++) {
    gc_vm_free_frame(vm->frames->data[i]);
  }
  gc_stack_free(vm->frames);

  for (int i = 0; i < vm->objects->count; i++) {
    gc_object_free(vm->frames->data[i]);
  }
  gc_stack_free(vm->objects);

  free(vm);
}

gc_vm_t *vm_new() {
  gc_vm_t *vm = malloc(sizeof(gc_vm_t));
  if (vm == NULL) {
    return NULL;
  }

  vm->frames = gc_stack_new(8);
  if (vm->frames == NULL) {
    free(vm);
    return NULL;
  }

  vm->objects = gc_stack_new(8);
  if (vm->objects == NULL) {
    free(vm);
    return NULL;
  }

  return vm;
}

// FIX : New free implemented?
// void gc_vm_free(gc_vm_t *vm) {
//   if (vm == NULL) {
//     return;
//   }
//
//   gc_stack_free(vm->frames);
//   gc_stack_free(vm->objects);
//   free(vm);
// }
