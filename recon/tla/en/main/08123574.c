// Under -mthumb-slow-loads (26 October 2026): differ 2 of 103 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it; none reaches 0.
// Near miss, differ 2 of 103 (4 plainly): The Broken Seal's
// BattleParty_CountStandbyDjinn. An empty barrier on the entry's timer
// puts its load before the -1 it is compared with, as the native. Left: the
// native stores the spilled count address to its stack slot (str r2, [sp])
// first in the loop's preheader, this draft after the zero for lr; sched2
// gives that zero's chain priority 2 and the store 1. Measured: i set inside
// the count test (differ 17), found set later (91), four minutes of permute
// (no gain).
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
      s32 timer = list->entries[i].timer;

      __asm__ volatile("" : "+l"(timer));
      if (timer == -1) {
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
