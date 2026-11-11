/// @file
/// Battle sprites: every unit's sprite placed, enemy sprite slots and the
/// party's weapon sprites.
// Part of battle.c.

#include "../battle_internal.h"

/// Readies every unit's battle graphics, releasing those of departed units,
/// and resets everyone's display mode and motion.
void BattleObject_Put(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

/// Readies every unit's battle graphics, releasing those of departed units,
/// and keeps their display modes and motions.
void BattleParty_ListAllUnitsAndSubmit(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 0);
}

s32 BattleMotion_GetSlotPalette(s32 id) { return BattleObject_Get(id)->palette; }

struct Layout {
  u8 pad[4];
  s16 field[6];
};

s32 Enemy_CheckRoom(s32 arg0) {
  s32 retval;
  s32 i;
  struct Layout *ptr;

  retval = BattleClass_IsSmall(arg0);
  ptr = (struct Layout *)Ram_HeapSlots->battleWork;
  i = 0;
  for (; i < 6; i++) {
    if (ptr->field[i] != 0)
      continue;
    if (retval != 0)
      break;
    if (i <= 4 && ptr->field[i + 1] == 0)
      break;
  }
  return i != 6;
}

struct BattleSpriteSlots {
  s32 enemyGroup;
  s16 spriteSlots[6];
};

s32 BattleClass_IsSmall(s32 index);
s32 BattleClass_GetSprite(s32 index);
s32 BattleClass_GetPalette(s32 index);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 number, u32 variant);

/// Loads the battle sprites of a unit into free sprite slots: one slot, or
/// two neighbouring ones for a wide sprite. A few sprites load a second part
/// into the next free slots. Returns the first slot in bits 12 and up with
/// the sprite value below, or 0 when a load fails.
// The Broken Seal's function of the same name, keyed by the enemy id, with
// seven two-part sprites.
s32 SummonSlot_RegisterActorSprites(s32 unit) {
  s32 pass;
  struct BattleSpriteSlots *table =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  struct BattleUnit *actor = Unit_Get(unit);
  s32 singleSlot = BattleClass_IsSmall(actor->enemyId);
  /* FAKEMATCH(temporary): result cleared a halfword at a time, which keeps
   * pass a copy of its register (mov r4, r10); as an s32 set to 0, 2 of 134
   * instructions differ in tla-en. */
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

    // The buffers' address held in a local, taken before the palette's call,
    // leaves r3 to the first result; inside the sum, 6 of 134 instructions
    // differ in tla-en.
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

void BattleMotion_ReleaseObjectSlotByValue(s32 value) {
  struct BattleSpriteSlots *work =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  s32 index;

  for (index = 0; index <= 5; index++) {
    if (work->spriteSlots[index] == value) {
      ResourceSlot_LoadFar(index, 0, 0, 0);
      work->spriteSlots[index] = 0;
    }
  }
}

s32 BattleUnit_GetWeaponKind(s32 key) {
  s32 index;
  s32 entry;
  s32 result;
  u32 value;

  for (index = 0;; index++) {
    entry = gResource_SlotAssignments[index];
    if (key == (entry & 0x1ff)) {
      /* FAKEMATCH(barrier): an empty asm clobbering memory, so the entry is
       * read again for the result; without it, 22 of 24 instructions differ in
       * tla-en. */
      asm("" : : : "memory");
      value = gResource_SlotAssignments[index];
      result = value >> 9;
      goto done;
    }
    if ((s16)entry == -1)
      break;
  }
  result = 6;
done:
  return result;
}

extern const u16 gData_081287d4[];
extern const u16 gData_081287e2[];
extern const u16 gData_081287f0[];
extern const u16 gData_081287fe[];
extern const u16 gData_0812880c[];
extern const u16 gData_0812881a[];
extern const u16 gData_08128828[];
extern const u16 gData_08128836[];

s32 Inventory_FindEquippedFar(s32 owner, s32 kind);

/// The value of the owner's equipped weapon from its class's table, or 0.
s32 BattleUnit_LookupWeaponValueByClass(s32 owner) {
  struct OwnerInventoryState *unit = Unit_Get(owner);
  s32 slot = Inventory_FindEquippedFar(owner, 1);
  s32 result = 0;

  if (slot >= 0) {
    s32 index;

    index = BattleUnit_GetWeaponKind(unit->inventory[slot] & 0x1ff);
    switch (unit->classId) {
    case ISAAC:
      result = gData_081287d4[index];
      break;
    case GARET:
      result = gData_081287e2[index];
      break;
    case IVAN:
      result = gData_081287f0[index];
      break;
    case MIA:
      result = gData_081287fe[index];
      break;
    case JENNA:
      result = gData_0812880c[index];
      break;
    case FELIX:
      result = gData_0812881a[index];
      break;
    case SHEBA:
      result = gData_08128828[index];
      break;
    case PIERS:
      result = gData_08128836[index];
      break;
    }
  }
  return result;
}
