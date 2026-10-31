// Near miss, differ 2 of 103 (tla-en, 2026-10-30): sched2's order alone. The
// native stores the spilled count address to its stack slot (str r2, [sp])
// first in the loop's preheader, this draft after the zero for lr and
// mov ip, r9. loop.c puts that hoisted address and the loop's induction
// starts together before the loop note, where the store has no dependent:
// sched2 ranks it 1 against the lr zero's chain of 2, so issuing it first
// needs a store cost of 2 or a barrier between them, which no source form
// places. The Broken Seal's BattleDjinn_CountStandby, as plain C: the empty
// barrier the 26 October draft kept on the entry's timer is no longer needed
// (differ 2 with or without it).
// Tried before, with that barrier, one edit at a time: an empty asm barrier
// after each statement, each adjacent pair of statements swapped, a register
// tie on each local after each statement, each local pinned to each
// register, each call argument copied into its argument register (pinned) at
// the call or up to three statements before it; none reaches 0.
// Measured: i set inside the count test (differ 17), found set later (91),
// the loop as a for or a while (2), wrapped in a once-only loop (2), a
// pointer to the count set after the test (69; 73 with a barrier after it),
// four minutes of permute on each form (no gain).
#include "types.h"
#include "battle_side.h"

struct PlacementEntry {
  u8 x;
  u8 y;
  u8 id;
  s8 timer;
};
struct PlacementList {
  struct PlacementEntry entries[72];
  s32 count;
};
struct PlacementTable {
  u8 padding[8];
  struct PlacementList list;
};

struct PlacementTable *Djinn_GetRecoveryTableFar(s32 owner);

s32 BattleParty_CountStandbyDjinn(u32 arg0, u8 *counts) {
  u16 values[8];
  struct PlacementList *list;
  s32 total;
  s32 found;
  s32 i;
  s32 j;
  s32 kind;
  s32 owner;

  found = 0;
  kind = 1;
  if (arg0 > 7)
    kind = 2;
  total = BattleParty_ListActorIds(kind, (u16 *)values);
  owner = 0;
  if (arg0 > 7)
    owner = 1;
  list = &Djinn_GetRecoveryTableFar(owner)->list;
  if (counts != 0)
    for (j = 3; j >= 0; j--)
      counts[j] = 0;
  i = 0;
  if (list->count != 0) {
    do {
      if (list->entries[i].timer == -1) {
        for (j = 0; j < total; j++)
          if (values[j] == list->entries[i].id)
            break;
        if (j != total) {
          if (counts != 0)
            counts[list->entries[i].x]++;
          found++;
        }
      }
      i++;
    } while (i != list->count);
  }
  return found;
}
