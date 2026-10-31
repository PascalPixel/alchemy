// Near miss, differ 2 of 84 (tla-en, 2026-10-30): instruction order only.
// sched2 issues the workspace address's first move (movs r3, #192) before the
// empty result's store (str r2, [sp, #4]); the native stores first. In the
// sched2 dump the store's priority (6) is below the address chain's (7), so
// no statement order reaches it; the permuter (4 minutes, 13,715 candidates)
// found nothing better.
// Func_080d5670's twin over the heap slot 0xa0 workspace's nodes: finds the
// used node nearest to a position, within a squared distance of 0x400
// (coordinates in 1/256 units, squared with the IWRAM Q16 multiply); NULL
// when none. The first of 080d5670's two barriers is gone: it stored the
// empty result first but forced the position's copy (mov sl, r0) above it,
// also differ 2. Without the remaining barrier, if-conversion (ifcvt case 2,
// the pointer test predicted taken) hoists the loop's set-up above the
// workspace test, differ 31. `if (work != NULL) { ... } return found;` needs
// no barrier but returns through the stored result instead of a plain NULL,
// differ 5. A volatile best or found, or keeping them in a struct or in
// one-element arrays, does worse.
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
