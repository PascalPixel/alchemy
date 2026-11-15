/// @file
/// Battle sprites: every unit's sprite placed, enemy sprite slots and the
/// party's weapon sprites.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "gamepak/far_call/field.h"
#include "battle/presentation.h"

/// Readies every unit's battle graphics, releasing those of departed units,
/// and resets everyone's display mode and motion.
void BattleObject_Put(void) {
  s16 list[UNIT_MAX];

  BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

/// Readies every unit's battle graphics, releasing those of departed units,
/// and keeps their display modes and motions.
void BattleUnit_ListAllAndSubmit(void) {
  s16 list[UNIT_MAX];

  BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 0);
}

/// Returns the palette of a unit's battle object.
s32 BattleMotion_GetSlotPalette(s32 id) {
  return BattleObject_Get(id)->palette;
}

/// The battle has six enemy sprite slots; a large enemy takes two side by
/// side, a small one one.
#define ENEMY_SPRITE_SLOTS 6

/// Returns 1 if an enemy of the class would find room among the enemy sprite
/// slots, two side by side for a large one.
s32 Enemy_HasSpriteRoom(s32 classId) {
  struct BattleWork *battle;
  s32 needsSingleSlot;
  s32 spriteSlot;

  needsSingleSlot = BattleSprite_IsSmall(classId);
  battle = Ram_HeapSlots->battleWork;
  for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
    if (battle->spriteSlots[spriteSlot] != 0)
      continue;
    if (needsSingleSlot != 0)
      break;
    if (spriteSlot < ENEMY_SPRITE_SLOTS - 1 &&
        battle->spriteSlots[spriteSlot + 1] == 0)
      break;
  }
  return spriteSlot != ENEMY_SPRITE_SLOTS;
}

/// Loads the battle sprites of a unit into free sprite slots: one slot, or
/// two neighbouring ones for a wide sprite. A few sprites load a second part
/// into the next free slots. Returns the first slot in bits 12 and up with
/// the sprite value below, or 0 when a load fails.
s32 EnemySprite_LoadIntoSlots(s32 unit) {
  s32 pass;
  struct BattleWork *table = Ram_HeapSlots->battleWork;
  struct Unit *actor = Unit_Get(unit);
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

    if (actor->classId != CLASS_NONE)
      continue;

    for (slot = 0; slot <= 5; slot++) {
      if (table->spriteSlots[slot] != 0)
        continue;
      if (singleSlot)
        break;
      if (slot <= 4 && table->spriteSlots[slot + 1] == 0)
        break;
    }

    if (slot == ENEMY_SPRITE_SLOTS)
      break;

    // The buffers' address held in a local, taken before the palette's call,
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

/// Frees every enemy sprite slot unit unitId holds, unloading its sprite.
/// Clears those BattleWork.spriteSlots.
void BattleObject_ReleaseSpriteSlots(s32 unitId) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 spriteSlot;

  for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
    if (battle->spriteSlots[spriteSlot] == unitId) {
      SpritePictureCache_LoadFar(spriteSlot, 0, 0, 0);
      battle->spriteSlots[spriteSlot] = 0;
    }
  }
}

// gBattleUnit_WeaponKinds: a weapon's item id with its kind above it.
#define WEAPON_ITEM_MASK 0x1ff
#define WEAPON_KIND_SHIFT 9

/// The kinds of weapon a party member's attack shows, by the rows of
/// gBattleUnit_WeaponKinds.
enum BattleWeaponKind {
  WEAPON_KIND_LONG_SWORD,
  WEAPON_KIND_LIGHT_BLADE,
  WEAPON_KIND_AXE,
  WEAPON_KIND_MACE,
  WEAPON_KIND_STAFF,
  WEAPON_KIND_ANKH,
  WEAPON_KIND_NONE ///< an item the list lacks
};

/// Returns the kind (enum BattleWeaponKind) of the weapon whose item id is
/// itemId, which picks a party member's weapon sprite.
s32 BattleUnit_GetWeaponKind(s32 itemId) {
  s32 assignmentIndex;
  u32 assignment;
  s32 weaponKind;

  for (assignmentIndex = 0;; assignmentIndex++) {
    if (itemId ==
        (gBattleUnit_WeaponKinds[assignmentIndex] & WEAPON_ITEM_MASK)) {
      assignment = gBattleUnit_WeaponKinds[assignmentIndex];
      weaponKind = assignment >> WEAPON_KIND_SHIFT;
      break;
    }
    if ((s16)gBattleUnit_WeaponKinds[assignmentIndex] == -1) {
      weaponKind = WEAPON_KIND_NONE;
      break;
    }
  }
  return weaponKind;
}

/// Returns the battle sprite of a character holding its equipped weapon, from
/// that character's table by the weapon's kind, or 0 with no weapon.
s32 BattleUnit_GetWeaponSprite(s32 owner) {
  struct Unit *unit = Unit_Get(owner);
  s32 slot = Inventory_FindEquippedFar(owner, EQUIP_WEAPON);
  s32 result = 0;

  if (slot >= 0) {
    s32 index;

    index = BattleUnit_GetWeaponKind(unit->inventory[slot] &
                                     INVENTORY_ITEM_ID_MASK);
    switch (unit->enemyId) {
    case ISAAC:
      result = gBattleUnit_WeaponAnimationsIsaac[index];
      break;
    case GARET:
      result = gBattleUnit_WeaponAnimationsGaret[index];
      break;
    case IVAN:
      result = gBattleUnit_WeaponAnimationsIvan[index];
      break;
    case MIA:
      result = gBattleUnit_WeaponAnimationsMia[index];
      break;
    case JENNA:
      result = gBattleUnit_WeaponAnimationsJenna[index];
      break;
    case FELIX:
      result = gBattleUnit_WeaponAnimationsFelix[index];
      break;
    case SHEBA:
      result = gBattleUnit_WeaponAnimationsSheba[index];
      break;
    case PIERS:
      result = gBattleUnit_WeaponAnimationsPiers[index];
      break;
    }
  }
  return result;
}
