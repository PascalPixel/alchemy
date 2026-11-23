/// @file
/// Battle sprites: every unit's sprite placed, enemy sprite slots and the
/// party's weapon sprites.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/game.h"
#include "battle/battle.h"
#include "gamepak/far_call/field.h"
#include "battle/presentation.h"

/// Readies every unit's sprites, destroying those of units no longer in the
/// battle, and sets each unit's pose again.
void BattleObject_Put(void) {
  s16 list[UNIT_MAX];

  BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, list);
  BattleActor_SpawnObjectsForList(list, 1);
}

/// Readies every unit's sprites, destroying those of units no longer in the
/// battle, and leaves the poses of units that have sprites as they are.
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

/// Each enemy sprite slot owns 16 KB of the sprite buffers, gActorSpriteSlots.
#define ENEMY_SPRITE_SLOT_SIZE 16384

/// Loads an enemy's battle sprite into the first free enemy sprite slots: one
/// slot, or two neighbouring ones for a large enemy. A few sprites load a
/// second part into the next free slots. Returns (slot << RESOURCE_SLOT_SHIFT)
/// | sprite, or 0 when a load fails.
s32 EnemySprite_LoadIntoSlots(s32 unitId) {
  s32 spritePart;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct Unit *unit = Unit_Get(unitId);
  s32 needsSingleSlot = BattleSprite_IsSmall(unit->enemyId);
  s32 result;
  s32 sprite;
  u8 *spriteBuffers;

  result = spritePart = 0;
  sprite = BattleSprite_GetSprite(unit->enemyId);
  // Once for the sprite, and again for its second part if it has one.
  for (spritePart = 0; spritePart <= 1; spritePart++) {
    s32 slot;

    // A unit with a class is no enemy: nothing is loaded and the result stays
    // 0.
    if (unit->classId != CLASS_NONE)
      continue;

    // The first free slot (and, for a large enemy, the one after it).
    for (slot = 0; slot < ENEMY_SPRITE_SLOTS; slot++) {
      if (battle->spriteSlots[slot] != 0)
        continue;
      if (needsSingleSlot)
        break;
      if (slot < ENEMY_SPRITE_SLOTS - 1 && battle->spriteSlots[slot + 1] == 0)
        break;
    }

    if (slot == ENEMY_SPRITE_SLOTS)
      break;

    spriteBuffers = gActorSpriteSlots;
    {
      s32 palette = BattleSprite_GetPalette(unit->enemyId);
      u8 *spriteBuffer = spriteBuffers + slot * ENEMY_SPRITE_SLOT_SIZE;

      if (SpritePictureCache_LoadFar(slot, (u32 *)spriteBuffer,
                                     sprite + spritePart, palette) == 0)
        return 0;
    }

    if (spritePart == 0)
      result = (slot << RESOURCE_SLOT_SHIFT) | sprite;

    battle->spriteSlots[slot] = unitId;
    if (!needsSingleSlot)
      battle->spriteSlots[slot + 1] = unitId;

    if (sprite != SPRITE_BATTLE_SERPENT && sprite != SPRITE_BATTLE_KRAKEN &&
        sprite != SPRITE_BATTLE_FUSION_DRAGON &&
        sprite != SPRITE_BATTLE_POSEIDON &&
        sprite != SPRITE_BATTLE_DOOM_DRAGON_A &&
        sprite != SPRITE_BATTLE_DOOM_DRAGON_E &&
        sprite != SPRITE_BATTLE_DOOM_DRAGON_H)
      break;
  }

  return result;
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

/// Returns the kind (enum BattleWeaponKind) of the weapon whose item id is
/// itemId, which picks a party member's weapon sprite.
s32 BattleUnit_GetWeaponKind(s32 itemId) {
  s32 row;
  u32 assignment;
  s32 weaponKind;

  for (row = 0;; row++) {
    if (itemId == (gBattleUnit_WeaponKinds[row] & WEAPON_ITEM_MASK)) {
      assignment = gBattleUnit_WeaponKinds[row];
      weaponKind = assignment >> WEAPON_KIND_SHIFT;
      break;
    }
    if ((s16)gBattleUnit_WeaponKinds[row] == -1) {
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
    s32 kind = BattleUnit_GetWeaponKind(unit->inventory[slot] &
                                        INVENTORY_ITEM_ID_MASK);
    switch (unit->enemyId) {
    case ISAAC:
      result = gBattleUnit_WeaponAnimationsIsaac[kind];
      break;
    case GARET:
      result = gBattleUnit_WeaponAnimationsGaret[kind];
      break;
    case IVAN:
      result = gBattleUnit_WeaponAnimationsIvan[kind];
      break;
    case MIA:
      result = gBattleUnit_WeaponAnimationsMia[kind];
      break;
    case JENNA:
      result = gBattleUnit_WeaponAnimationsJenna[kind];
      break;
    case FELIX:
      result = gBattleUnit_WeaponAnimationsFelix[kind];
      break;
    case SHEBA:
      result = gBattleUnit_WeaponAnimationsSheba[kind];
      break;
    case PIERS:
      result = gBattleUnit_WeaponAnimationsPiers[kind];
      break;
    }
  }
  return result;
}
