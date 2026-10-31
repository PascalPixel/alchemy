// Near miss, differ 4 of 158 (tla-en, 2026-10-31; 8 on 2026-10-26): scheduler
// order only, in the last block. The native loads `lost` before `removed` and
// issues `add sp` before the last store. Here sched2 takes the reload of
// `removed` first (it follows the `*gained` store at cost 1, a better class
// than `lost`'s anti-dependent load), and the USE of the return register takes
// the cycle after the loads, so the store, earlier in RTL, wins the tie with
// `add sp` (a void function gets that slot, as 080d7b04 does). Unsigned
// counters, return or out-pointer types and an empty asm before `count++;`
// leave it at 4; storing `*lost` first gives 12.
#include "battle_system.h"

struct DjinnSlot {
  u16 id;
  u16 state;
};

s32 DjinnMenu_ListChangedDjinn(struct DjinnSlot *oldGrid,
                               struct DjinnSlot *newGrid, u16 *out, s32 *gained,
                               s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 added;
  s32 removed;

  added = 0;
  removed = 0;
  count = 0;
  for (i = 0; i < 32 && newGrid[i].id != 0; i++) {
    if (BattleAction_Get(newGrid[i].id)->flags & 0x80) {
      out[count] = newGrid[i].id & 0x3fff;
      count++;
      for (j = 0; j < 32; j++) {
        if (((newGrid[i].id ^ oldGrid[j].id) & 0x3fff) == 0)
          break;
      }
      if (j == 32) {
        added++;
        out[count - 1] |= 0x8000;
      }
    }
  }
  for (i = 0; i < 32 && oldGrid[i].id != 0; i++) {
    if (BattleAction_Get(oldGrid[i].id)->flags & 0x80) {
      for (j = 0; j < 32; j++) {
        if (((oldGrid[i].id ^ newGrid[j].id) & 0x3fff) == 0)
          break;
      }
      if (j == 32) {
        removed++;
        out[count] = (oldGrid[i].id & 0x3fff) | 0x4000;
        count++;
      }
    }
  }
  *gained = added;
  *lost = removed;
  return count;
}
