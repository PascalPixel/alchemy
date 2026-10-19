#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "battle_summon.h"
#include "types.h"

/// The summons in the order the menus list them.
extern u8 gSummon_OrderList[29];

u32 *Djinn_GetRecoveryTable();

/// Lists the summons, in the game's order, whose bit is set in the state's
/// first word, ends the list with 32 and returns how many it listed.
s32 Summon_ListLearned(u8 *output) {
  u8 *source = gSummon_OrderList;
  u8 *end = gSummon_OrderList + 28;
  s32 count = 0;
  /* FAKEMATCH: the one is kept in a register across the loop, as the
     native keeps it in r8; written as a constant in the shift, with the
     word pinned below, it is built in the loop (differ 18). */
  u32 one = 1;

  do {
    u8 value = *source;
    /* FAKEMATCH: the word is pinned to the r3 it takes in the native, so
       the and keeps its result there; in a plain local the word and the
       bit trade registers (differ 10). */
    register u32 flags asm("r3") = *Djinn_GetRecoveryTable(0);

    /* FAKEMATCH: the barrier loads the word before the bit is started; as
       plain C, the copy of the one comes first and 2 instructions differ
       in tla-en. */
    asm("");
    source++;
    flags &= one << value;
    if (flags != 0) {
      *output++ = value;
      count++;
    }
  } while (source <= end);
  *output = 32;
  return count;
}

const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId) {
  if (summonId > 28)
    return NULL;
  return &gSummon_DefinitionTable[summonId];
}

void Func_080aff94(s32 bit) {
  u32 *state = Djinn_GetRecoveryTable(0);
  /* FAKEMATCH: the word pinned to r3 is loaded before the bit is built; in a
   * plain local, 10 instructions differ in tla-en. */
  register u32 flags __asm__("r3") = *state;

  /* FAKEMATCH: the one-pass block's loop note holds the bit until the pinned
   * word is loaded. */
  do {
  } while (0);
  flags |= 1 << bit;
  *state = flags;
}
