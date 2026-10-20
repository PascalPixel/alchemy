/// @file
/// Battle objects made and placed on the battlefield.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// Isaac and Garet hold a Machete with sprites of their own.
#define SPRITE_ISAAC_MACHETE 480
#define SPRITE_GARET_MACHETE 482

/// BattleObj.scale and MotionObject scales are 16.16 fixed point.
#define SCALE_ONE 0x10000

/// BattleObj.effect while no effect is attached.
#define BATTLE_OBJ_NO_EFFECT 0x1fe

/// Creates the battle object of unit at (x, y) on the battlefield, in
/// pixels: its sprite (an enemy's from its class, sharing the slot it
/// already holds; a party member's battle sprite), palette, size and weapon
/// sprite, turned to face the centre, and starts its motion script
/// (gBattlePres_ActorObjectScript). Called by BattleUnit_RefreshPlacement
/// and by BattleTargetExec for an enemy that joins. Fills in actor, creates
/// a field object for it and, for an enemy, claims its sprite slots.
void BattleObjMake(struct BattleObj *actor, s32 unitId, s32 x, s32 y) {
  s32 fixedX;
  s32 fixedZ;
  s32 palette;
  s32 sprite;
  s32 registeredSprite;
  struct BattleUnit *unit;
  struct MotionObject *object;
  s32 facingAngle;
  s32 weaponSprite;
  u8 classId;

  // Positions are 16.16 fixed point; y is the battlefield's depth.
  fixedX = x * 65536;
  fixedZ = y * 65536;
  // Kind 15 skips FieldObject_Create's sprite creation. Battle sprites are
  // attached later by BattleActor_SpawnObjectsForList.
  object = (struct MotionObject *)Object_CreateFar(0xf000, fixedX, 0, fixedZ);
  unit = UnitGetFar(unitId);
  palette = 0;
  registeredSprite = SummonSlot_RegisterActorSprites(unitId);

  // The sprite: an enemy's from its class (classIndex 0), a party member's
  // by character. An enemy whose sprite went into its own slots takes the
  // slot's sprite and palette 0, else its class's sprite and palette; a
  // character fighting on the enemy side (a link opponent, unit id 8 or
  // above) takes palette 1.
  if (unit->classIndex == 0) {
    sprite = BattleClass_GetSprite(unit->classId);
    if (registeredSprite == 0)
      palette = BattleClass_GetPalette(unit->classId);
    else
      sprite = registeredSprite;
  } else {
    switch (unit->classId) {
    case GARET:
      sprite = BATTLE_CHARACTER(GARET);
      break;
    case MIA:
      sprite = BATTLE_CHARACTER(MIA);
      break;
    case IVAN:
      sprite = BATTLE_CHARACTER(IVAN);
      break;
    case JENNA:
      sprite = BATTLE_CHARACTER(JENNA);
      break;
    case ISAAC:
    default:
      sprite = BATTLE_CHARACTER(ISAAC);
      break;
    }
    if (unitId < 0 || unitId > 7)
      palette = 1;
  }

  // The enemy classes drawn larger or smaller than their sprites, in 16.16
  // (0x18000 is 1.5, 0xe666 0.9). Grouping equal scale cases shortens the
  // matching EN function by 148 bytes, so each case keeps its own store.
  // Class ids are enemies.tsv rows plus ENEMY_CLASS_FIRST.
  actor->scale = SCALE_ONE;
  switch (unit->classId) {
  case 78:
    actor->scale = 0x19999;
    break;
  case 89:
    actor->scale = 0x18ccc;
    break;
  case 130:
    actor->scale = 0x13333;
    break;
  case 131:
    actor->scale = 0x19999;
    break;
  case 138:
    actor->scale = 0x18000;
    break;
  case 147:
    actor->scale = 0x1cccc;
    break;
  case 149:
    actor->scale = 0x1cccc;
    break;
  case 29:
    actor->scale = 0x10000;
    break;
  case 121:
    actor->scale = 0x1b333;
    break;
  case 148:
    actor->scale = 0x18000;
    break;
  case 150:
    actor->scale = 0x18000;
    break;
  case 151:
    actor->scale = 0x18000;
    break;
  case 152:
    actor->scale = 0x18000;
    break;
  case 153:
    actor->scale = 0x18000;
    break;
  case 154:
    actor->scale = 0x18000;
    break;
  case 155:
    actor->scale = 0x18000;
    break;
  case 156:
    actor->scale = 0x18000;
    break;
  case 157:
    actor->scale = 0x18000;
    break;
  case 47:
    actor->scale = 0x13333;
    break;
  case 48:
    actor->scale = 0x13333;
    break;
  case 49:
    actor->scale = 0x16666;
    break;
  case 84:
    actor->scale = 0x10000;
    break;
  case 85:
    actor->scale = 0x14000;
    break;
  case 128:
    actor->scale = 0x16666;
    break;
  case 129:
    actor->scale = 0x16666;
    break;
  case 94:
    actor->scale = 0x18000;
    break;
  case 98:
    actor->scale = 0x14ccc;
    break;
  case 110:
    actor->scale = 0x13333;
    break;
  case 132:
    actor->scale = 0x10ccc;
    break;
  case 133:
    actor->scale = 0x10ccc;
    break;
  case 134:
    actor->scale = 0x11999;
    break;
  case 135:
    actor->scale = 0x11999;
    break;
  case 136:
    actor->scale = 0x13333;
    break;
  case 137:
    actor->scale = 0x13333;
    break;
  case 141:
    actor->scale = 0x18000;
    break;
  case 144:
    actor->scale = 0x13333;
    break;
  case 145:
    actor->scale = 0x18000;
    break;
  case 146:
    actor->scale = 0x18ccc;
    break;
  case 52:
    actor->scale = 0x14000;
    break;
  case 105:
    actor->scale = 0x14000;
    break;
  case 18:
  case 19:
  case 20:
  case 21:
  case 30:
  case 68:
  case 69:
  case 70:
  case 92:
  case 122:
  case 123:
  case 124:
  case 125:
  case 126:
    actor->scale = 0xe666;
    break;
  }

  actor->object = object;
  actor->anchorX = fixedX;
  actor->anchorZ = fixedZ;
  actor->palette = palette;
  actor->resource = sprite;
  weaponSprite = BattleUnit_LookupWeaponValueByClass(unitId);
  actor->animation = 0;
  actor->animationEntry = 0;
  actor->effectEntry = 0;
  actor->active = 0;
  actor->fading = 0;
  actor->effect = BATTLE_OBJ_NO_EFFECT;
  classId = unit->classId;
  actor->overlay = weaponSprite;

  // Isaac and Garet draw a Machete as part of their own sprite, with no
  // weapon overlay.
  if (classId <= GARET &&
      Inventory_GetEquippedItemFar(unit, EQUIP_WEAPON) == ITEM_MACHETE) {
    if (unit->classId == ISAAC) {
      sprite = SPRITE_ISAAC_MACHETE;
      actor->resource = sprite;
    } else {
      sprite = SPRITE_GARET_MACHETE;
      actor->resource = sprite;
    }
    actor->overlay = 0;
  }

  // Face the centre of the battlefield (0x8000 is half a turn, so the
  // angle points back along the line to the origin); enemies are drawn at
  // 1.3 times their size. motionFlags 2 lets the object fall.
  facingAngle = ArcTan2(y / 8, x) + 0x8000;
  object->angle = facingAngle;
  object->unk59 = 3;
  object->motionFlags = 2;
  if (unit->classIndex == 0) {
    object->scaleX = 0x14ccc;
    object->scaleY = 0x14ccc;
  } else {
    object->scaleX = SCALE_ONE;
    object->scaleY = SCALE_ONE;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gBattlePres_ActorObjectScript);
}

/// Reads the battlefield position of party place index from
/// gBattlePlacement_StepPairs, in pixels. Nothing in the source calls it;
/// BattleUnit_RefreshPlacement reads the table directly.
void BattlePlacement_GetStepPair(s32 partyPlace, s32 *x, s32 *y) {
  partyPlace *= 2;
  *x = gBattlePlacement_StepPairs[partyPlace];
  *y = gBattlePlacement_StepPairs[partyPlace + 1];
}

/// BattleWork.enemyIds of an enemy that left the battle, and of a place
/// held for one: unit ids 254 and 255 in the list.
#define IS_PLACEHOLDER_ID(id) ((u16)((id) - BATTLE_UNIT_REMOVED) <= 1)

/// Lays out count enemies in a column 80 pixels left of the centre, spaced
/// by their size (38 pixels around a large enemy, 27 around a small one, 25
/// around an empty place, closer when there are more than four) and centred
/// on the line. Two large classes stand nearer, 50 pixels left. Called by
/// BattleUnit_RefreshPlacement and EnemyUpdate; writes only xPositions and
/// zPositions.
void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions,
                            s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  // z starts half a line's estimated length up and steps down by half of
  // each enemy's spacing before it and half after it.
  for (index = 0; index != count; index++) {
    s32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;

      spacing = 25;
      if (!IS_PLACEHOLDER_ID(actorIds[index])) {
        actor = UnitGetFar(actorIds[index]);
        spacing = BattleClass_IsSmall(actor->classId) ? 27 : 38;
        if (actor->classId == 148 || actor->classId == 121) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if (!IS_PLACEHOLDER_ID(actorIds[index])) {
      struct BattleUnit *actor = UnitGetFar(actorIds[index]);

      spacing = BattleClass_IsSmall(actor->classId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}

/// Returns the id of the first free enemy unit, or -1 when all six are taken.
/// Called by BattleTargetExec when an enemy calls for help.
s32 EnemySlotGet(void) {
  s32 enemySlot;
  s32 unitId;

  for (enemySlot = 0; enemySlot < ENEMY_BATTLE_SIZE; enemySlot++) {
    unitId = enemySlot + FIRST_ENEMY_UNIT;
    if (UnitGetFar(unitId)->side == BATTLE_UNIT_ABSENT)
      break;
  }
  if (enemySlot == ENEMY_BATTLE_SIZE)
    return -1;
  return unitId;
}

/// Lays the enemies out again and moves each one's object anchor to its new
/// place. Called by BattleTargetExec after an enemy joins. Writes the
/// anchorX and anchorZ of every enemy's battle object.
#ifdef UBFIX
void EnemyUpdate(void) {
#else
s32 EnemyUpdate(void) {
#endif
  struct BattleWork *battle = gBattleWork;
  u16 unitIds[14];
  s32 xPositions[6];
  s32 zPositions[6];
  s32 enemyCount;
  s32 enemyIndex;

  for (enemyIndex = 0; enemyIndex < ENEMY_BATTLE_SIZE &&
                       battle->enemyIds[enemyIndex] != BATTLE_UNIT_LIST_END;
       enemyIndex++)
    unitIds[enemyIndex] = battle->enemyIds[enemyIndex];
  enemyCount = enemyIndex;
  Summon_LayoutPositions(unitIds, enemyCount, xPositions, zPositions);
  for (enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++) {
    s32 unitId = battle->enemyIds[enemyIndex];

    if (unitId != BATTLE_UNIT_REMOVED) {
      struct BattleObj *object = BattleObjGet(unitId);

      object->anchorX = xPositions[enemyIndex] * 65536;
      object->anchorZ = zPositions[enemyIndex] * 65536;
    }
  }
}

/// Places every unit at the start of a battle: numbers the places (party
/// members 0 up in their order, enemies 8 to 13) and creates each unit's
/// object at its spot. Called by Battle_RunEncounter once the unit lists are
/// filled. Writes gBattleWork->placement and every unit's battle object.
void BattleUnit_RefreshPlacement(void) {
  struct BattleWork *battle;
  u16 unitIds[14];
  s32 xPositions[6];
  s32 zPositions[6];
  s32 count;
  s32 unitIndex;
  s32 partyPlace;
  s32 enemyPlace;
  s32 remainingPlaces;
  s32 placementValue;
  u8 *placementCursor;

  partyPlace = 0;
  enemyPlace = 0;
  battle = gBattleWork;
  count = BattleParty_PrepareActiveOwners(unitIds);
  // placement[0..7] is each party unit id's place (0xff for none) and
  // placement[8..13] the enemy places, numbered 8 to 13.
  placementValue = 0xff;
  for (unitIndex = 13; unitIndex >= 0; unitIndex--)
    battle->placement[unitIndex] = placementValue;
  placementCursor = &battle->placement[13];
  for (remainingPlaces = 5, placementValue = 13; remainingPlaces >= 0;
       remainingPlaces--, placementValue--)
    *placementCursor-- = placementValue;

  // The party, at its fixed spots.
  for (unitIndex = 0; unitIndex < count; partyPlace++, unitIndex++) {
    s32 unitId = unitIds[unitIndex];

    battle->placement[unitId] = partyPlace;
    BattleObjMake(BattleObjGet(unitId), unitId,
                  gBattlePlacement_StepPairs[partyPlace * 2],
                  gBattlePlacement_StepPairs[partyPlace * 2 + 1]);
  }

  // The enemies, laid out by size.
  for (unitIndex = 0; unitIndex < ENEMY_BATTLE_SIZE &&
                      battle->enemyIds[unitIndex] != BATTLE_UNIT_LIST_END;
       unitIndex++)
    unitIds[unitIndex] = battle->enemyIds[unitIndex];
  count = unitIndex;
  Summon_LayoutPositions(unitIds, count, xPositions, zPositions);
  for (unitIndex = 0; unitIndex < count; unitIndex++, enemyPlace++) {
    s32 unitId = battle->enemyIds[unitIndex];

    if (unitId != BATTLE_UNIT_REMOVED)
      BattleObjMake(BattleObjGet(unitId), unitId, xPositions[enemyPlace],
                    zPositions[enemyPlace]);
  }
}

/// Returns 1 if the unit at place is in list. Called by
/// BattleActor_SpawnObjectsForList to find the objects to release.
/// @param list unit ids, ended by BATTLE_UNIT_LIST_END, 14 at most
/// @param place 0 to 7 a party unit id, 8 to 13 the enemies
s32 BattlePlacement_ContainsId(s16 *list, s32 place) {
  u32 listIndex;

  // An enemy's place to its unit id.
  if (place > 7)
    place += FIRST_ENEMY_UNIT - 8;
  listIndex = 0;
  do {
    u16 unitId = *list++;

    if (unitId == BATTLE_UNIT_LIST_END)
      break;
    if (unitId == place)
      return 1;
  } while (++listIndex < 14);
  return 0;
}
