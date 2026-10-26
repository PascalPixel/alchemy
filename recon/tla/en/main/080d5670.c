// Under -mthumb-slow-loads (2026-10-26): differ 2 of 85 in tla-en, instruction
// order only. Without the two barriers it differs 33; no placement of one to
// three empty asms does better than the draft's two. Copying position to a
// second variable after the workspace load changes nothing.
// Near miss, differ 6 of 85: finds the used item of the heap slot 0x9c workspace
// nearest to a position, within a squared distance of 0x400 (coordinates taken
// in 1/256 units, squared with the IWRAM Q16 multiply); NULL when none. No ☀️
// twin found. The count runs up (loop.c reverses it to 127 down, giving the
// native registers: counting down in the source swaps the count and the
// IWRAM entry, differ 22); two tagged barriers keep the empty result's
// store first and the loop's set-up after the workspace test. Left:
// instruction order only: the native copies the position (mov sl, r0) after
// the workspace load, not before it, issues the IWRAM entry's pool load before
// the best distance's shift, and shifts the first difference (asrs r1) after
// the third coordinate's load.
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
  /* FAKEMATCH: the barrier stores the empty result before the workspace is
   * loaded (differ 8 without it). */
  asm("");
  work = WORK_9C;
  if (work == NULL)
    return NULL;
  /* FAKEMATCH: the barrier keeps the loop's set-up (the item pointer, the
   * best distance, the count and the IWRAM entry) after the workspace test,
   * where if-conversion otherwise hoists it (differ 35 without it). */
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
