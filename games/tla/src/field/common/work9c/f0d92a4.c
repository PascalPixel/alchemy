#include "work9c.h"

void Func_080d92a4(s32 index, s32 *position) {
  struct Work9c *work = WORK_9C;
  struct Work9cSlot *slot = &work->slots[index];
  struct Work9cNode *node = slot->head;
  s32 depth;

  depth = slot->depth;
  if (depth > 0) {
    do {
      depth--;
      node = node->next;
    } while (depth != 0);
  }
  position[0] = node->x;
  position[1] = node->y;
  position[2] = node->z;
}
