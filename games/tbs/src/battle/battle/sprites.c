/// @file
/// Battle sprites: enemy sprite slots and the party's weapon sprites.
// Part of battle.c.

#include "../battle_internal.h"

/// Readies the sprite of every unit in the battle (releasing those of units
/// that left) and sets each one's display mode and motion afresh. Called by
/// Battle_Run as the battle is set up, by
/// BattleStaging_DispatchAction, BattleStaging_HideField,
/// the auto battle and BattleAbility_ApplyToTarget after a call for help, and
/// through the far stub BattleObject_PutFar by BattleAnim_RunParticleStreams.
void BattleObject_Put(void) {
  s16 unitIds[14];

  BattleUnit_ListIds(BATTLE_SIDES_BOTH, unitIds);
  BattleObject_SpawnObjectsForList(unitIds, 1);
}

/// Readies the sprite of every unit in the battle (releasing those of units
/// that left), keeping their modes and motions. Called by
/// BattleStaging_RunAttack.
void BattleUnit_ListAllAndSubmit(void) {
  s16 unitIds[14];

  BattleUnit_ListIds(BATTLE_SIDES_BOTH, unitIds);
  BattleObject_SpawnObjectsForList(unitIds, 0);
}

/// Returns the palette of unit id's battle object. Called by
/// BattlePlayback_Run and BattleMotion_FlashHit, and through its far stub
/// by the battle animation dispatcher (battle_anim/parts/dispatch.c).
s32 BattleMotion_GetSlotPalette(s32 id) {
  return BattleObject_Get(id)->palette;
}

/// The battle has six enemy sprite slots; a large enemy takes two side by
/// side, a small one (BattleSprite_IsSmall) one.
#define ENEMY_SPRITE_SLOTS 6

/// Returns 1 if an enemy of class classId would find room among the enemy
/// sprite slots: one free slot for a small enemy, two free side by side for
/// a large one. Called by BattleAbility_ApplyToTarget before an enemy calls for
/// help.
s32 Enemy_HasSpriteRoom(s32 classId) {
  struct BattleWork *battle;
  s32 needsSingleSlot;
  s32 spriteSlot;

  needsSingleSlot = BattleSprite_IsSmall(classId);
  battle = gBattleWork;
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

/// Loads an enemy unit's battle sprite into the first free enemy sprite
/// slots (one for a small enemy, two side by side for a large one) and
/// marks them as the unit's. Two sprites, the Kraken's and the Fusion
/// Dragon's, take a second part, loaded the same way into further slots.
/// The slots stay the unit's until BattleObject_ReleaseSlotByValue
/// frees them as it leaves the battle. Returns the first slot and the
/// sprite as (slot << 12) | sprite, or 0 for a party member's unit or when
/// a load fails. Called by BattleObject_Make. Writes gBattleWork->spriteSlots
/// and the slots' 16 KB sprite buffers at Ram_EnemySpriteSlots.
s32 EnemySprite_LoadIntoSlots(s32 unitId) {
  s32 spritePart;
  struct BattleWork *battle = gBattleWork;
  struct Unit *unit = Unit_GetFar(unitId);
  s32 needsSingleSlot = BattleSprite_IsSmall(unit->id);
  /* FAKEMATCH(temporary): result cleared a halfword at a time, which keeps
   * spritePart a copy of its register (mov r4, r10); as an s32 set to 0, 1 of
   * 114 instructions differ in tbs-en. */
  union {
    s32 value;
    u16 halves[2];
  } result;
  s32 sprite;

  result.halves[0] = 0;
  result.halves[1] = 0;
  sprite = BattleSprite_GetSprite(unit->id);
  spritePart = result.value;
  // Once for the sprite, and again for its second part if it has one.
  do {
    s32 slot;

    // A unit with a classIndex is no enemy: nothing is loaded and the
    // result stays 0.
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

    {
      s32 palette = BattleSprite_GetPalette(unit->id);
      // Each slot owns 16 KB of sprite buffer.
      s32 spriteBuffer = (slot << 14) + (s32)Ram_EnemySpriteSlots;

      if (SpriteFrameCache_LoadFar(slot, (u32 *)spriteBuffer,
                                   sprite + spritePart, palette) == 0)
        return 0;
    }

    /* FAKEMATCH(dead): two updates that cancel, raising sprite's priority in
     * allocation; without them, 9 of 114 instructions differ in tbs-en. */
    sprite ^= 1;
    sprite ^= 1;
    if (spritePart == 0)
      result.value = (slot << 12) | sprite;

    battle->spriteSlots[slot] = unitId;
    if (!needsSingleSlot)
      battle->spriteSlots[slot + 1] = unitId;

    if (sprite != SPRITE_WITH_SECOND_PART_A &&
        sprite != SPRITE_WITH_SECOND_PART_B)
      break;
  } while (++spritePart <= 1);

  return result.value;
}

/// Frees every enemy sprite slot unit unitId holds, unloading its sprite.
/// Called by BattleObject_MarkForRelease. Clears those
/// gBattleWork->spriteSlots.
#ifdef UBFIX
void BattleObject_ReleaseSlotByValue(s32 unitId) {
#else
s32 BattleObject_ReleaseSlotByValue(s32 unitId) {
#endif
  struct BattleWork *battle = gBattleWork;
  s32 spriteSlot;

  for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
    if (battle->spriteSlots[spriteSlot] == unitId) {
      SpriteFrameCache_LoadFar(spriteSlot, 0, 0, 0);
      battle->spriteSlots[spriteSlot] = 0;
    }
  }
}

// gResource_SlotAssignments: each entry is a weapon's item id in the low nine
// bits and its kind above them; the list ends with -1. Each class's table of
// weapon sprites has one entry a kind.
#define WEAPON_ITEM_MASK 0x1ff
#define WEAPON_KIND_SHIFT 9

/// The kinds of weapon a party member's attack shows, by the rows of
/// items.tsv gResource_SlotAssignments lists for each.
enum BattleWeaponKind {
  WEAPON_KIND_LONG_SWORD,  ///< Long Sword to Muramasa
  WEAPON_KIND_LIGHT_BLADE, ///< Machete to Bandit's Sword
  WEAPON_KIND_AXE,         ///< Battle Axe to Demon Axe
  WEAPON_KIND_MACE,        ///< Mace to Wicked Mace
  /// Wooden Stick to Shaman's Rod, but the two ankhs among them.
  WEAPON_KIND_STAFF,
  WEAPON_KIND_ANKH, ///< the Blessed Ankh and Angelic Ankh
  WEAPON_KIND_NONE  ///< an item the list lacks
};

/// Returns the kind (enum BattleWeaponKind) of the weapon whose item id is
/// itemId, which picks a party member's weapon sprite. Called by
/// BattleUnit_LookupWeaponValueByClass.
s32 BattleUnit_GetWeaponKind(s32 itemId) {
  s32 assignmentIndex;
  u32 assignment;
  s32 weaponKind;

  for (assignmentIndex = 0;; assignmentIndex++) {
    if (itemId ==
        (gResource_SlotAssignments[assignmentIndex] & WEAPON_ITEM_MASK)) {
      assignment = gResource_SlotAssignments[assignmentIndex];
      weaponKind = assignment >> WEAPON_KIND_SHIFT;
      break;
    }
    if ((s16)gResource_SlotAssignments[assignmentIndex] == -1) {
      weaponKind = WEAPON_KIND_NONE;
      break;
    }
  }
  return weaponKind;
}

/// Returns the weapon sprite a party member's attack shows for the kind of
/// weapon they have equipped (gBattleUnit_WeaponAnimations tables), or 0 for no
/// weapon, for Felix, or for a unit that is no party member. Called by
/// BattleObject_Make, which keeps it in BattleObject.overlay for
/// BattleObject_SpawnObjectsForList to add as a layer of the body sprite.
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId) {
  struct Unit *unit;
  s32 slot;
  s32 weaponSprite;

  unit = Unit_GetFar(unitId);
  slot = Inventory_FindEquippedFar(unitId, EQUIP_WEAPON);
  weaponSprite = 0;
  if (slot >= 0) {
    s32 weaponKind;

    weaponKind =
        BattleUnit_GetWeaponKind(unit->inventory[slot] & WEAPON_ITEM_MASK);
    switch (unit->id) {
    case ISAAC:
      weaponSprite = gBattleUnit_WeaponAnimationsIsaac[weaponKind];
      break;
    case GARET:
      weaponSprite = gBattleUnit_WeaponAnimationsGaret[weaponKind];
      break;
    case IVAN:
      weaponSprite = gBattleUnit_WeaponAnimationsIvan[weaponKind];
      break;
    case MIA:
      weaponSprite = gBattleUnit_WeaponAnimationsMia[weaponKind];
      break;
    case FELIX:
      break;
    case JENNA:
      weaponSprite = gBattleUnit_WeaponAnimationsJenna[weaponKind];
      break;
    }
  }
  return weaponSprite;
}
