#include "work9c.h"

// Returns the state of slot index of the heap slot 0x9c work, or -1 while the
// work is not allocated.
s32 Func_080d9ab0(s32 index) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot;

  if (work == NULL)
    return -1;
  slot = &work->slots[index];
  return slot->state;
}
