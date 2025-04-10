#include "gc_stack.h"

typedef struct VirtualMachine {
  gc_stack_t *frames;
  gc_stack_t *objects;
} vm_t;

vm_t *vm_new();
void vm_free(vm_t *vm);
void vm_frame_push();
gc_frame_t *vm_new_frame(vm_t *vm);
void gc_frame_free(frame_t *frame);
