// Draft of Func_081281ec, The Broken Seal's BattleSpoils_Clear
// (games/tbs/src/battle/battle.c): differ 2 of 27 (tla-en, 2026-10-15).
// The battle's spoils sit at 0x550 of The Lost Age's battle work, and the
// pending item is the party state's halfword at 0x25c.

// Left: one row. The native loads the party state's address from the pool
// between the spoils offset's shift and its add (movs, lsls, ldr, adds);
// here the add comes first: the pooled load issued one slot later, the
// residue of this lane's other near misses. Tried: the spoils cleared
// before the pending item (16), the spoils pointer taken after the pending
// item (30).

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_session.h"

// Clears the battle's spoils: coins, experience, the defeated count and
// the four dropped items, and the item waiting to be given.
void Func_081281ec(void) {
  u8 *battle = (u8 *)Ram_HeapSlots->battleWork;
  struct BattleSpoils *spoils = (struct BattleSpoils *)(battle + 0x550);
  u16 *items;
  s32 index;

  gPartyState.tally25c = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = spoils->items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}
