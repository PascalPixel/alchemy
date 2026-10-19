#include "types.h"
#include "ram_buffer.h"
#include "battle_unit.h"

struct BattleSpriteSlots {
  s32 enemyGroup;
  s16 spriteSlots[6];
};

/// The actor sprite slot buffers, 0x4000 bytes a slot.
extern u8 Data_02018000[];

struct BattleUnit *UnitGet(s32 unit);
s32 BattleClass_IsSmall(s32 index);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

/// Loads the battle sprites of a unit into free sprite slots: one slot, or
/// two neighbouring ones for a wide sprite. A few sprites load a second part
/// into the next free slots. Returns the first slot in bits 12 and up with
/// the sprite value below, or 0 when a load fails. The Broken Seal's function
/// of the same name, keyed by the enemy id, with seven two-part sprites.
s32 SummonSlot_RegisterActorSprites(s32 unit) {
  s32 pass;
  struct BattleSpriteSlots *table =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  struct BattleUnit *actor = UnitGet(unit);
  s32 singleSlot = BattleClass_IsSmall(actor->enemyId);
  /* FAKEMATCH(temporary): result cleared a halfword at a time, which keeps
   * pass a copy of its register (mov r4, r10), as in The Broken Seal; as an
   * s32 set to 0, 2 of 134 instructions differ in tla-en. */
  union {
    s32 value;
    u16 halves[2];
  } result;
  s32 spriteValue;
  s32 slotBuffers;

  result.halves[0] = 0;
  result.halves[1] = 0;
  spriteValue = BattleClass_GetSprite(actor->enemyId);
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

    /* FAKEMATCH(temporary): the buffers' address taken before the flag's
     * call, which builds the first result in r3; as the address inside the
     * sum, 6 of 134 instructions differ in tla-en. */
    slotBuffers = (s32)Data_02018000;
    {
      s32 flag = BattleClass_GetPalette(actor->enemyId);
      s32 bufferAddr = (slot << 14) + slotBuffers;

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
