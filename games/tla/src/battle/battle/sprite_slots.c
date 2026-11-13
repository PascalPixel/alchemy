/// @file
/// Battle sprites: every unit's sprite placed, enemy sprite slots and the
/// party's weapon sprites.
// Part of battle.c.

#include "../battle_internal.h"
#include "gamepak/far_call/game.h"
#include "module/battle/battle.h"
#include "gamepak/far_call/field.h"
#include "module/battle/presentation.h"

/// Readies every unit's battle graphics, releasing those of departed units,
/// and resets everyone's display mode and motion.
void BattleObject_Put(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

/// Readies every unit's battle graphics, releasing those of departed units,
/// and keeps their display modes and motions.
void BattleUnit_ListAllAndSubmit(void) {
  s16 list[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 0);
}

s32 BattleMotion_GetSlotPalette(s32 id) {
  return BattleObject_Get(id)->palette;
}

s32 Enemy_HasSpriteRoom(s32 arg0) {
  s32 retval;
  s32 i;
  struct Layout *ptr;

  retval = BattleSprite_IsSmall(arg0);
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

/// Loads the battle sprites of a unit into free sprite slots: one slot, or
/// two neighbouring ones for a wide sprite. A few sprites load a second part
/// into the next free slots. Returns the first slot in bits 12 and up with
/// the sprite value below, or 0 when a load fails.
// The Broken Seal's function of the same name, keyed by the enemy id, with
// seven two-part sprites.
s32 EnemySprite_LoadIntoSlots(s32 unit) {
  s32 pass;
  struct BattleSpriteSlots *table =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  struct BattleUnit *actor = Unit_Get(unit);
  s32 singleSlot = BattleSprite_IsSmall(actor->enemyId);
  /* FAKEMATCH(temporary): result cleared a halfword at a time. */
  union {
    s32 value;
    u16 halves[2];
  } result;
  s32 spriteValue;
  s32 slotBuffers;

  result.halves[0] = 0;
  result.halves[1] = 0;
  spriteValue = BattleSprite_GetSprite(actor->enemyId);
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
      s32 flag = BattleSprite_GetPalette(actor->enemyId);
      s32 bufferAddr = (slot << 14) + slotBuffers;

      if (SpritePictureCache_LoadFar(slot, (u32 *)bufferAddr,
                                     spriteValue + pass, flag) == 0)
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

void BattleObject_ReleaseSpriteSlots(s32 value) {
  struct BattleSpriteSlots *work =
      (struct BattleSpriteSlots *)Ram_HeapSlots->battleWork;
  s32 index;

  for (index = 0; index <= 5; index++) {
    if (work->spriteSlots[index] == value) {
      SpritePictureCache_LoadFar(index, 0, 0, 0);
      work->spriteSlots[index] = 0;
    }
  }
}

s32 BattleUnit_GetWeaponKind(s32 key) {
  s32 index;
  u32 value;
  s32 result;

  for (index = 0;; index++) {
    if (key == (gBattleUnit_WeaponKinds[index] & 0x1ff)) {
      value = gBattleUnit_WeaponKinds[index];
      result = value >> 9;
      break;
    }
    if ((s16)gBattleUnit_WeaponKinds[index] == -1) {
      result = 6;
      break;
    }
  }
  return result;
}

/// The value of the owner's equipped weapon from its class's table, or 0.
s32 BattleUnit_GetWeaponSprite(s32 owner) {
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
      result = gBattleUnit_WeaponAnimationsIsaac[index];
      break;
    case JENNA:
      result = gBattleUnit_WeaponAnimationsGaret[index];
      break;
    case FELIX:
      result = gBattleUnit_WeaponAnimationsIvan[index];
      break;
    case SHEBA:
      result = gBattleUnit_WeaponAnimationsMia[index];
      break;
    case PIERS:
      result = gBattleUnit_WeaponAnimationsJenna[index];
      break;
    }
  }
  return result;
}
