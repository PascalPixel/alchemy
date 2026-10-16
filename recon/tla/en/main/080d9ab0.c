// Near miss, differ 9: the listing keeps the index in r0 and loads the work
// into r2; this draft loads the work into r0 and copies the index out of its
// way.
#include "work9c.h"

s32 Func_080d9ab0(s32 index) {
  struct Work9c *work = WORK_9C;

  if (work == NULL)
    return -1;
  return work->slots[index].state;
}
