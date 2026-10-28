#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "battle_work.h"

u8 *Item_Get(u32);

// XXX: what this encodes, and whether item ids here are wider than The Broken
// Seal's 9 bits; its callers would settle it.
u32 BattleSpoils_RankItem(u32 value) {
  u32 bank = 0;
  u32 base = value & 0x1ff;
  if (base == 0)
    return 0;
  {
    u8 flags = Item_Get(base)[3];

    if (flags & 8)
      bank = 1;
    bank <<= 1;
    if (flags & 4)
      bank++;
    bank <<= 9;
    bank += base;
  }
  return bank;
}

/// Empties the battle's spoils and the party's pending item, as The Broken
/// Seal's BattleSpoils_Clear does. Called by Func_081197f0.
void BattleSpoils_Clear(void) {
  struct BattleWork *battle;
  struct BattleSpoils *spoils;
  u16 *items;
  s32 index;

  battle = Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  gMainRoutineWork.tally25c = 0;
  spoils->coins = 0;
  spoils->experience = 0;
  spoils->defeated = 0;
  items = battle->spoils.items;
  for (index = 3; index >= 0; index--)
    items[index] = 0;
}
