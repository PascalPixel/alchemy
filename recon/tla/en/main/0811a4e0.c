// Draft of Func_0811a4e0, The Broken Seal's SummonSlot_RegisterActorSprites
// (games/tbs/src/battle/battle.c): differ 6 of 134 (tla-en, 2026-10-15).
// The Lost Age keys the sprites by the enemy id at 0x14a and lists seven
// sprite values whose actors load a second part, where The Broken Seal lists
// two. The twin's union of two halves stays: it keeps the copy of the zero
// into pass (mov r4, r10), as there.

// Left: one register. The native builds the first slot's result (slot << 12
// | spriteValue) in r3 before moving it to r10; reload takes r2 here.
// Tried: the twin's cancelling XOR pair (6), the OR's operands swapped (6),
// a block-local value or shifted local (6), a u32 cast (6), result as a
// plain s32 (8), the store after the slot table's (71), result.halves[0]
// (43), a u16 spriteValue (122), a u32 slot (10), spriteValue declared
// before the union (6).

#include "types.h"
#include "ram_buffer.h"
#include "battle_unit.h"

struct BattleSpriteSlots {
  s32 enemyGroup;
  s16 spriteSlots[6];
};

extern u8 Ram_ActorSpriteSlots[];

struct BattleUnit *UnitGet(s32 unit);
s32 Summon_IsEntryFlagged(s32 index);
s32 Summon_GetEntryValue(s32 index);
s32 Summon_GetEntryFlag1Field(s32 index);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

// Loads the battle sprites of a unit into free sprite slots: one slot, or
// two neighbouring ones for a wide sprite. A few sprites load a second part
// into the next free slots. Returns the first slot in bits 12 and up with
// the sprite value below, or 0 when a load fails.
s32 Func_0811a4e0(s32 unit) {
  s32 pass;
  struct BattleSpriteSlots *table =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  struct BattleUnit *actor = UnitGet(unit);
  s32 singleSlot = Summon_IsEntryFlagged(actor->enemyId);
  union {
    s32 value;
    u16 halves[2];
  } result;
  s32 spriteValue;

  result.halves[0] = 0;
  result.halves[1] = 0;
  spriteValue = Summon_GetEntryValue(actor->enemyId);
  pass = result.value;
  do {
    s32 slot;

    if (actor->classIndex != 0)
      continue;

    for (slot = 0; slot <= 5; slot++) {
      if (table->spriteSlots[slot] != 0)
        continue;
      if (singleSlot)
        break;
      if (slot <= 4 && table->spriteSlots[slot + 1] == 0)
        break;
    }

    if (slot == 6)
      break;

    {
      s32 flag = Summon_GetEntryFlag1Field(actor->enemyId);
      s32 bufferAddr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

      if (ResourceSlot_LoadFar(slot, (u32 *)bufferAddr, spriteValue + pass,
                               flag) == 0)
        return 0;
    }

    if (pass == 0)
      result.value = (slot << 12) | spriteValue;

    table->spriteSlots[slot] = unit;
    if (!singleSlot)
      table->spriteSlots[slot + 1] = unit;

    if (spriteValue != 561 && spriteValue != 631 && spriteValue != 638 &&
        spriteValue != 585 && spriteValue != 579 && spriteValue != 581 &&
        spriteValue != 583)
      break;
  } while (++pass <= 1);

  return result.value;
}
