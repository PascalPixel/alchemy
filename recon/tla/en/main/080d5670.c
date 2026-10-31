// Near miss, differ 2 of 85 (tla-en, 2026-10-30): instruction order only, the
// scheduler's cost model. The native stores the empty result (str r2, [sp, #4])
// before it builds the workspace address (movs r3, #192); here sched2 issues
// the address first: its chain to the workspace load has priority 7, the
// store's 6 (a store before a load that may miss costs 2 under
// -mthumb-slow-loads, the address's three steps 3). An empty asm between the
// store and the workspace load restores that order but pins the position's
// copy (mov sl, r0) above it, before the load where the native issues it:
// also differ 2, with a second device. Copying position to a second variable
// after that asm changes nothing (gcse's copy propagation folds the copy back).
// Earlier notes still true: finds the used item of the heap slot 0x9c workspace
// nearest to a position, within a squared distance of 0x400 (coordinates taken
// in 1/256 units, squared with the IWRAM Q16 multiply); NULL when none. No ☀️
// twin found. The count runs up (loop.c reverses it to 127 down, giving the
// native registers: counting down in the source swaps the count and the
// IWRAM entry, differ 22). Without the remaining barrier, the if-conversion
// pass (ce, IF-CASE-2) hoists the loop's set-up above the workspace test.
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
  /* FAKEMATCH(barrier): the empty asm keeps the loop's set-up (the item
   * pointer, the best distance, the count and the IWRAM entry) after the
   * workspace test, where if-conversion would hoist it; as plain C without
   * it, 31 of 85 instructions differ in tla-en. */
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
