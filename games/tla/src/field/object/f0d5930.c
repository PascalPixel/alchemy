#include "types.h"
#include "iwram_call.h"
#include "worka0.h"

struct WorkA0Node *Func_080d5930(s32 *position) {
  struct WorkA0Node *found;
  struct WorkA0 *work;
  struct WorkA0Node *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_A0;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): the empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) below the
   * workspace test; as the plain early return, 31 of 84 instructions differ
   * in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      s32 dx = (position[0] - item->x) >> 8;
      s32 dy = (position[1] - item->y) >> 8;
      s32 dz = (position[2] - item->z) >> 8;
      s32 distance =
          Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);

      if (distance <= 0x400 && best > distance) {
        best = distance;
        found = item;
      }
    }
  }
  return found;
}
