#include "types.h"
#include "iwram_call.h"
#include "work9c.h"

struct Work9cItemPosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct Work9cItem *Func_080d5670(s32 *position) {
  struct Work9cItem *found;
  struct Work9c *work;
  struct Work9cItem *item;
  s32 best;
  s32 i;

  found = NULL;
  work = WORK_9C;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH(barrier): an empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; without it, 29 of 85
   * instructions differ in tla-en. */
  asm("");
  item = work->items;
  best = 0x10000000;
  for (i = 0; i < 128; i++, item++) {
    if (item->used == 1) {
      struct Work9cItemPosition *at = (struct Work9cItemPosition *)item;
      s32 dx = (position[0] - at->x) >> 8;
      s32 dy = (position[1] - at->y) >> 8;
      s32 dz = (position[2] - at->z) >> 8;
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
