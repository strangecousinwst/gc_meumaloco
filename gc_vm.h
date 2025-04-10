#pragma once
#include "gc_object.h"
#include "gc_stack.h"

typedef struct GC_VirtualMachine {
  gc_stack_t *frames;
  gc_stack_t *objects;
} gc_vm_t;

typedef struct GC_StackFrame {
  gc_stack_t *references;
} gc_frame_t;

gc_vm_t *gc_vm_new();
void gc_vm_free(gc_vm_t *vm);
void gc_vm_track_object(gc_vm_t *vm, gc_object_t *obj);

void gc_vm_push_frame(gc_vm_t *vm, gc_frame_t *frame);
gc_frame_t *gc_vm_new_frame(gc_vm_t *vm);

void gc_vm_free_frame(gc_frame_t *frame);

void gc_mark(gc_vm_t *vm);
