#include "work9c.h"

struct Work9cItem *Func_080d8d40(void) {
  struct Work9c *work = WORK_9C;
  struct Work9cItem *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}
