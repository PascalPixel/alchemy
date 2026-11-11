#include "battle_system.h"

struct DjinnSlot {
  u16 id;
  u16 state;
};

/// Lists into `out` the abilities usable in battle (flag 0x80) of the action
/// list `newGrid`, in its order, marking with bit 15 those `oldGrid` lacks;
/// then appends those of `oldGrid` that `newGrid` lacks, marked with bit 14.
/// Both lists are 32 slots, ended early by a 0 action. Returns the entries
/// written, and the gained and lost counts through `gained` and `lost`.
s32 BattleDjinnMenu_ListChangedDjinn(struct DjinnSlot *oldGrid,
                                     struct DjinnSlot *newGrid, u16 *out,
                                     s32 *gained, s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 gainedCount;
  s32 lostCount;

  gainedCount = 0;
  lostCount = 0;
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
        gainedCount++;
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
        lostCount++;
        out[count] = (oldGrid[i].id & 0x3fff) | 0x4000;
        count++;
      }
    }
  }
  *gained = gainedCount;
  *lost = lostCount;
  return count;
}
