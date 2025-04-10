#include "gc_vm.h"
#include "gc_stack.h"
#include "gc_object.h"


void gc_mark(gc_vm_t *vm) {
  if (vm == NULL) { return; }

  for (int i = 0; i < vm->frames->count; i++) {
    gc_frame_t *frame = vm->frames[i];
    if (frame == NULL) { continue; }

    for (int j = 0; j < frame->references->count; j++) {
      gc_object_t *obj = frame->references->data[j];
      if (obj == NULL) { continue; }
      obj->is_marked = true;
    }
  }
  

}

void gc_vm_frame_reference_object(gc_frame_t *frame, gc_object_t *obj) {
  if (frame == NULL || obj == NULL) { return; }
  gc_stack_push(frame->references, (void *)obj);
}

void gc_vm_push_frame(gc_vm_t *vm, gc_frame_t *frame) {
  if (vm == NULL || frame == NULL) { return; }
  gc_stack_push(vm->frames, (void *)frame);
}

gc_frame_t *gc_vm_new_frame(gc_vm_t *vm) {
  gc_frame_t *frame = malloc(sizeof(gc_frame_t));
  if (frame == NULL) { return NULL; }

  frame->references = stack_new(8);
  if (frame->references == NULL) {
    free(frame);
    return NULL;
  }

  gc_vm_push_frame(vm, frame);
  return frame;
}

void gc_vm_free_frame(gc_frame_t *frame) {
  if (frame == NULL) { return NULL; }

  gc_stack_free(frame->references);
  free(frame);
}

void gc_vm_free(gc_vm_t *vm) {
  if (vm == NULL) { return; }
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
  if (vm == NULL) { return NULL; }

  vm->frames = stack_new(8);
  if (vm->frames == NULL) {
    free(vm);
    return NULL;
  }

  vm->objects = stack_new(8);
  if (vm->objects == NULL) {
    free(vm);
    return NULL;
  }

  return vm;
}

void gc_vm_free(gc_vm_t *vm) {
  if (vm == NULL) { return; }

  gc_stack_free(vm->frames);
  gc_stack_free(vm->objects);
  free(vm);
}
