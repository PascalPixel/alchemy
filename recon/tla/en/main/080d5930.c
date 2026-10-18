// Draft, differ 47 of 84: Func_080d5670 over the heap slot 0xa0 workspace's
// nodes: finds the used node nearest to a position, within a squared distance
// of 0x400 (coordinates in 1/256 units, squared with the IWRAM Q16 multiply);
// NULL when none. Left, as in 080d5670: the native tests the workspace first
// and only then starts the best distance, the 127 count and the IWRAM entry,
// which agscc hoists above the test; register choice (the native keeps the
// entry in r11 and the count in r9 and copies the position pointer to r2).
#include "types.h"
#include "iwram_call.h"
#include "worka0.h"

struct WorkA0NodePosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct WorkA0Node *Func_080d5930(s32 *position) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *found = NULL;
  struct WorkA0Node *item;
  s32 best = 0x10000000;
  s32 i;

  if (work == NULL)
    return NULL;
  item = work->items;
  for (i = 127; i >= 0; i--, item++) {
    if (item->used == 1) {
      struct WorkA0NodePosition *at = (struct WorkA0NodePosition *)item;
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
