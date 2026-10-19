// Part of battle.c, the battle object, compiled only through it.
// Battle sprites: enemy sprite slots and the party's weapon sprites.

#include "../battle_internal.h"

// Readies the sprite of every unit in the battle (releasing those of units
// that left) and sets each one's display mode and motion afresh.
void BattleObjPut(void) {
  s16 unitIds[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  BattleActor_SpawnObjectsForList(unitIds, 1);
}

// Readies the sprite of every unit in the battle (releasing those of units
// that left), keeping their modes and motions.
void BattleParty_ListAllUnitsAndSubmit(void) {
  s16 unitIds[14];

  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  BattleActor_SpawnObjectsForList(unitIds, 0);
}

// Returns the palette of unit id's battle sprite.
s32 BattleMotion_GetSlotPalette(s32 id) { return BattleObjGet(id)->palette; }

// The battle has six enemy sprite slots; a large enemy takes two side by
// side, a small one (BattleClass_IsSmall) one.
#define ENEMY_SPRITE_SLOTS 6

// Returns 1 if an enemy of class classId would find room among the enemy
// sprite slots: one free slot for a small enemy, two free side by side for
// a large one.
s32 EnemyCheck(s32 classId) {
  struct BattleWork *battle;
  s32 needsSingleSlot;
  s32 spriteSlot;

  needsSingleSlot = BattleClass_IsSmall(classId);
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

// Loads an enemy unit's battle sprite into the first free enemy sprite
// slots (one for a small enemy, two side by side for a large one) and
// records the unit as their owner. Two sprites take a second part,
// loaded the same way into further slots. Returns the first slot and the
// sprite as (slot << 12) | sprite, or 0 for a party member's unit or when
// a load fails.
s32 SummonSlot_RegisterActorSprites(s32 unitId) {
  s32 spritePart;
  struct BattleWork *battle = gBattleWork;
  struct BattleUnit *unit = UnitGetFar(unitId);
  s32 needsSingleSlot = BattleClass_IsSmall(unit->classId);
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
  sprite = BattleClass_GetSprite(unit->classId);
  spritePart = result.value;
  do {
    s32 slot;

    if (unit->classIndex != 0)
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
      s32 palette = BattleClass_GetPalette(unit->classId);
      s32 spriteBuffer = (slot << 14) + (s32)Ram_ActorSpriteSlots;

      if (ResourceSlot_LoadFar(slot, (u32 *)spriteBuffer, sprite + spritePart,
                               palette) == 0)
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

// Frees every enemy sprite slot unit unitId holds, unloading its sprite.
s32 BattleMotion_ReleaseObjectSlotByValue(s32 unitId) {
  struct BattleWork *battle = gBattleWork;
  s32 spriteSlot;

  for (spriteSlot = 0; spriteSlot < ENEMY_SPRITE_SLOTS; spriteSlot++) {
    if (battle->spriteSlots[spriteSlot] == unitId) {
      ResourceSlot_LoadFar(spriteSlot, 0, 0, 0);
      battle->spriteSlots[spriteSlot] = 0;
    }
  }
}

// gResource_SlotAssignments: each entry is a weapon's item id in the low nine
// bits and its kind above them; the list ends with -1.
#define WEAPON_ITEM_MASK 0x1ff
#define WEAPON_KIND_SHIFT 9
#define WEAPON_KIND_NONE 6

// Returns the kind of the weapon whose item id is itemId (0 to 5), which
// picks a party member's attack animation, or WEAPON_KIND_NONE for an item
// the list lacks.
s32 Resource_FindFreeSlot(s32 itemId) {
  struct SlotAssignmentView {
    volatile u16 entries[52];
  };
  s32 assignmentIndex;
  s32 assignment;
  u32 reloadedAssignment;
  s32 weaponKind;

  for (assignmentIndex = 0;; assignmentIndex++) {
    assignment = gResource_SlotAssignments[assignmentIndex];
    if (itemId == (assignment & WEAPON_ITEM_MASK)) {
      /* FAKEMATCH(volatile): the entry read again through a volatile view,
       * a second ldrh as in the ROM; through the plain array the first load is
       * reused, and 11 of 24 instructions differ in tbs-en. */
      reloadedAssignment =
          ((struct SlotAssignmentView *)gResource_SlotAssignments)
              ->entries[assignmentIndex];
      weaponKind = reloadedAssignment >> WEAPON_KIND_SHIFT;
      break;
    }
    if ((s16)assignment == -1) {
      weaponKind = WEAPON_KIND_NONE;
      break;
    }
  }
  return weaponKind;
}

// Returns the weapon sprite a party member's attack shows for the kind of
// weapon they have equipped (gBattleUnit_WeaponAnimsClass*), or 0 for no
// weapon, for Felix, or for a unit that is no party member.
s32 BattleUnit_LookupWeaponValueByClass(s32 unitId) {
  struct BattleUnit *unit;
  s32 slot;
  s32 weaponSprite;

  unit = UnitGetFar(unitId);
  slot = Inventory_FindEquippedFar(unitId, EQUIP_WEAPON);
  weaponSprite = 0;
  if (slot >= 0) {
    s32 weaponKind;

    weaponKind =
        Resource_FindFreeSlot(unit->inventory[slot] & WEAPON_ITEM_MASK);
    switch (unit->classId) {
    case ISAAC:
      weaponSprite = gBattleUnit_WeaponAnimsClass0[weaponKind];
      break;
    case GARET:
      weaponSprite = gBattleUnit_WeaponAnimsClass1[weaponKind];
      break;
    case IVAN:
      weaponSprite = gBattleUnit_WeaponAnimsClass2[weaponKind];
      break;
    case MIA:
      weaponSprite = gBattleUnit_WeaponAnimsClass3[weaponKind];
      break;
    case FELIX:
      break;
    case JENNA:
      weaponSprite = gBattleUnit_WeaponAnimsClass5[weaponKind];
      break;
    }
  }
  return weaponSprite;
}
