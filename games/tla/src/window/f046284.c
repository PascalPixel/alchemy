#include "battle_system.h"

struct DjinnSlot {
  u16 id;
  u16 state;
};

void Func_08046284(struct DjinnSlot *oldGrid, struct DjinnSlot *newGrid,
                   u16 *out, s32 *gained, s32 *lost) {
  /* FAKEMATCH(signature): declared void, the count left in r0; as s32
     returning count, 3 of 158 instructions differ in tla-en. */
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
  {
    /* FAKEMATCH(pin): the count pinned to r0; as a plain local, 1 of 158
       instructions differ in tla-en. */
    register s32 result asm("r0") = count;

    /* FAKEMATCH(barrier): an empty asm reading the pinned count; without it,
       27 of 158 instructions differ in tla-en. */
    asm("" : : "r"(result));
  }
  *lost = removed;
}
