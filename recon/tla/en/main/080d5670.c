// Draft, differ 47 of 85: finds the used item of the heap slot 0x9c workspace
// nearest to a position, within a squared distance of 0x400 (coordinates taken
// in 1/256 units, squared with the IWRAM Q16 multiply); NULL when none. No ☀️
// twin found. Left: the native tests the workspace first and only then starts
// the best distance, the 127 count and the IWRAM entry; agscc hoists those
// above the test. Register choice: the native keeps the entry in r11 and the
// count in r9 and copies the position pointer to r2 for its loads, this draft
// the other way round. The entry held in a local function pointer differs 37
// but bypasses the IWRAM call layer; the loop with its pointer in the for
// differs 48.
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
  struct Work9c *work = WORK_9C;
  struct Work9cItem *found = NULL;
  struct Work9cItem *item;
  s32 best = 0x10000000;
  s32 i;

  if (work == NULL)
    return NULL;
  item = work->items;
  for (i = 127; i >= 0; i--, item++) {
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
