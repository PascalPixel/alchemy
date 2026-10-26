// Under -mthumb-slow-loads (2026-10-26): differ 2 of 84 in tla-en, instruction
// order only. As Func_080d5670.
// Near miss, differ 6 of 84: Func_080d5670 over the heap slot 0xa0 workspace's
// nodes: finds the used node nearest to a position, within a squared distance
// of 0x400 (coordinates in 1/256 units, squared with the IWRAM Q16 multiply);
// NULL when none. Written as 080d5670, with its two tagged barriers. Left, as
// there: instruction order only (the position's copy, the IWRAM entry's pool
// load one slot later than the native, the first shift).
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
  struct WorkA0Node *found;
  struct WorkA0 *work;
  struct WorkA0Node *item;
  s32 best;
  s32 i;

  found = NULL;
  /* FAKEMATCH: the barrier stores the empty result before the workspace is
   * loaded (differ 8 without it). */
  asm("");
  work = WORK_A0;
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
