/// @file
/// Battle objects made and placed on the battlefield.
// Part of battle.c.

#include "../battle_internal.h"

/// Isaac and Garet hold a Machete with sprites of their own.
#define SPRITE_ISAAC_MACHETE 480
#define SPRITE_GARET_MACHETE 482

/// BattleObject.effect as a battle object is made: the sprite
/// BattleObject_SpawnObjectsForList adds as a layer of every one-part body
/// (NARROW_EFFECT_SPRITE on a body 32 pixels wide). Its 64 by 64 picture
/// (graphics/sprite/spr_510.png) holds only a 32 by 6 ellipse along its
/// bottom edge, NARROW_EFFECT_SPRITE's a 16 by 3 one.
/// XXX: likely the unit's shadow on the ground rather than an effect, because
/// of that picture, because adding it also clears the sprite's own shadow
/// (AnimationObject.flags bit 0), and because
/// BattleObject_UpdateSpritePriorities changes the layer's priority while the
/// unit is off the ground.
#define BATTLE_OBJ_NO_EFFECT 0x1fe

/// Creates the battle object of unit at (x, y) on the battlefield, in
/// pixels: its sprite (an enemy's from its class, sharing the slot it
/// already holds; a party member's battle sprite), palette, size and weapon
/// sprite, turned to face the centre, and starts its motion script
/// (gBattlePresentation_ActorObjectScript). Called by
/// BattleUnit_RefreshPlacement and by BattleTarget_Execute for an enemy that
/// joins. Fills in actor, creates a field object for it and, for an enemy,
/// claims its sprite slots.
void BattleObject_Make(struct BattleObject *actor, s32 unitId, s32 x, s32 y) {
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
  // The id 0xf000 has no sprite kind of enum ObjectCreateKind, so
  // FieldObject_Create makes no sprite; BattleObject_SpawnObjectsForList
  // attaches the battle sprites later.
  object = (struct MotionObject *)Object_CreateFar(0xf000, fixedX, 0, fixedZ);
  unit = Unit_GetFar(unitId);
  palette = 0;
  registeredSprite = SummonSlot_RegisterActorSprites(unitId);

  // The sprite: an enemy's from its class (CLASS_INDEX_ENEMY), a party member's
  // by character. An enemy whose sprite went into its own slots takes the
  // slot's sprite and palette 0, else its class's sprite and palette; a
  // character fighting on the enemy side (a link opponent, a unit id above
  // ALLY_MAX) takes palette 1.
  if (unit->classIndex == CLASS_INDEX_ENEMY) {
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
    if (unitId < 0 || unitId > ALLY_MAX)
      palette = 1;
  }

  // The enemy classes drawn larger or smaller than their sprites, in 16.16
  // (0x18000 is 1.5, 0xe666 0.9). Grouping equal scale cases shortens the
  // matching EN function by 148 bytes, so each case keeps its own store.
  // Bandit and Orc Captain are set to 1.0 again.
  actor->scale = Q16_ONE;
  switch (unit->classId) {
  case ENEMY_KILLER_APE:
    actor->scale = 0x19999;
    break;
  case ENEMY_MANTICORE:
    actor->scale = 0x18ccc;
    break;
  case ENEMY_LIVING_STATUE:
    actor->scale = 0x13333;
    break;
  case ENEMY_HYDROS_STATUE:
    actor->scale = 0x19999;
    break;
  case ENEMY_ANT_LION:
    actor->scale = 0x18000;
    break;
  case ENEMY_TRET:
    actor->scale = 0x1cccc;
    break;
  case ENEMY_DEADBEARD:
    actor->scale = 0x1cccc;
    break;
  case ENEMY_BANDIT:
    actor->scale = 0x10000;
    break;
  case ENEMY_KRAKEN:
    actor->scale = 0x1b333;
    break;
  case ENEMY_FUSION_DRAGON:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_2:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_3:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_4:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_5:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_6:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_7:
    actor->scale = 0x18000;
    break;
  case ENEMY_MIMIC_8:
    actor->scale = 0x18000;
    break;
  case ENEMY_TROLL:
    actor->scale = 0x13333;
    break;
  case ENEMY_CAVE_TROLL:
    actor->scale = 0x13333;
    break;
  case ENEMY_BRUTAL_TROLL:
    actor->scale = 0x16666;
    break;
  case ENEMY_ORC_CAPTAIN:
    actor->scale = 0x10000;
    break;
  case ENEMY_ORC_LORD:
    actor->scale = 0x14000;
    break;
  case ENEMY_LIZARD_FIGHTER:
    actor->scale = 0x16666;
    break;
  case ENEMY_LIZARD_KING:
    actor->scale = 0x16666;
    break;
  case ENEMY_HOBGOBLIN:
    actor->scale = 0x18000;
    break;
  case ENEMY_GRYPHON:
    actor->scale = 0x14ccc;
    break;
  case ENEMY_CHIMERA:
    actor->scale = 0x13333;
    break;
  case ENEMY_AZART:
    actor->scale = 0x10ccc;
    break;
  case ENEMY_AZART_2:
    actor->scale = 0x10ccc;
    break;
  case ENEMY_SATRAGE:
    actor->scale = 0x11999;
    break;
  case ENEMY_SATRAGE_2:
    actor->scale = 0x11999;
    break;
  case ENEMY_NAVAMPA:
    actor->scale = 0x13333;
    break;
  case ENEMY_NAVAMPA_2:
    actor->scale = 0x13333;
    break;
  case ENEMY_TOADONPA:
    actor->scale = 0x18000;
    break;
  case ENEMY_TORNADO_LIZARD:
    actor->scale = 0x13333;
    break;
  case ENEMY_STORM_LIZARD:
    actor->scale = 0x18000;
    break;
  case ENEMY_TEMPEST_LIZARD:
    actor->scale = 0x18ccc;
    break;
  case ENEMY_RECLUSE:
    actor->scale = 0x14000;
    break;
  case ENEMY_CEREBUS:
    actor->scale = 0x14000;
    break;
  case ENEMY_SLIME:
  case ENEMY_SLIME_2:
  case ENEMY_OOZE:
  case ENEMY_SLIME_BEAST:
  case ENEMY_THIEF:
  case ENEMY_SIREN:
  case ENEMY_SUCCUBUS:
  case ENEMY_NIGHTMARE:
  case ENEMY_KOBOLD:
  case ENEMY_MYSTERY_MAN:
  case ENEMY_SATUROS:
  case ENEMY_SATUROS_2:
  case ENEMY_MYSTERY_WOMAN:
  case ENEMY_MENARDI:
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
  // 1.3 times their size. ACTOR_FALLS lets the object fall.
  facingAngle = ArcTan2(y / 8, x) + 0x8000;
  object->angle = facingAngle;
  object->unk59 = 3;
  object->motionFlags = ACTOR_FALLS;
  if (unit->classIndex == CLASS_INDEX_ENEMY) {
    object->scaleX = 0x14ccc;
    object->scaleY = 0x14ccc;
  } else {
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
  ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                               gBattlePresentation_ActorObjectScript);
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
/// by their size: each takes 38 pixels of the line if large, 27 if small and
/// 25 for an empty place, half before and half after it, none before the
/// first. The column starts from an estimate of its length, 30 pixels an
/// enemy (27 for more than four), so it sits only roughly centred. The
/// Kraken and the Fusion Dragon would stand nearer, 50 pixels left, past the
/// first place; each is fought alone (formatn.tsv rows 213 and 347), so
/// neither does. Called by BattleUnit_RefreshPlacement and Enemy_Update;
/// writes only xPositions and zPositions, in pixels.
void BattlePlacement_LayoutEnemies(u16 *actorIds, s32 count, s32 *xPositions,
                                   s32 *zPositions) {
  s32 totalSpacing = count <= 4 ? 30 : 27;
  s32 z = ((count - 1) * totalSpacing) / 2;
  s32 index;

  // z starts half a line's estimated length up and steps down by half of
  // each enemy's spacing before it (none for the first) and half after it.
  for (index = 0; index != count; index++) {
    s32 spacing = 0;

    xPositions[index] = -80;
    if (index != 0) {
      struct BattleUnit *actor;

      spacing = 25;
      if (!IS_PLACEHOLDER_ID(actorIds[index])) {
        actor = Unit_GetFar(actorIds[index]);
        spacing = BattleClass_IsSmall(actor->classId) ? 27 : 38;
        // The first place keeps -80 whatever its class.
        if (actor->classId == ENEMY_FUSION_DRAGON ||
            actor->classId == ENEMY_KRAKEN) {
          xPositions[index] = -50;
        }
      }
    }
    z -= spacing / 2;
    zPositions[index] = z;
    spacing = 25;
    if (!IS_PLACEHOLDER_ID(actorIds[index])) {
      struct BattleUnit *actor = Unit_GetFar(actorIds[index]);

      spacing = BattleClass_IsSmall(actor->classId) ? 27 : 38;
    }
    z -= spacing / 2;
  }
}

/// Returns the id of the first free enemy unit, or -1 when all six are taken.
/// Called by BattleTarget_Execute when an enemy calls for help.
s32 Enemy_GetFreeSlot(void) {
  s32 enemySlot;
  s32 unitId;

  for (enemySlot = 0; enemySlot < ENEMY_BATTLE_SIZE; enemySlot++) {
    unitId = enemySlot + FIRST_ENEMY_UNIT;
    if (Unit_GetFar(unitId)->side == BATTLE_UNIT_ABSENT)
      break;
  }
  if (enemySlot == ENEMY_BATTLE_SIZE)
    return -1;
  return unitId;
}

/// Lays the enemies out again and moves each one's object anchor to its new
/// place. Called by BattleTarget_Execute after an enemy joins. Writes the
/// anchorX and anchorZ of every enemy's battle object.
#ifdef UBFIX
void Enemy_Update(void) {
#else
s32 Enemy_Update(void) {
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
  BattlePlacement_LayoutEnemies(unitIds, enemyCount, xPositions, zPositions);
  for (enemyIndex = 0; enemyIndex < enemyCount; enemyIndex++) {
    s32 unitId = battle->enemyIds[enemyIndex];

    if (unitId != BATTLE_UNIT_REMOVED) {
      struct BattleObject *battleObject = BattleObject_Get(unitId);

      battleObject->anchorX = xPositions[enemyIndex] * 65536;
      battleObject->anchorZ = zPositions[enemyIndex] * 65536;
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

  // The party, at its fixed spots in pixels: x 80, z 48, 16, -16 and -48
  // (gBattlePlacement_StepPairs, battle_data.s).
  for (unitIndex = 0; unitIndex < count; partyPlace++, unitIndex++) {
    s32 unitId = unitIds[unitIndex];

    battle->placement[unitId] = partyPlace;
    BattleObject_Make(BattleObject_Get(unitId), unitId,
                      gBattlePlacement_StepPairs[partyPlace * 2],
                      gBattlePlacement_StepPairs[partyPlace * 2 + 1]);
  }

  // The enemies, laid out by size.
  for (unitIndex = 0; unitIndex < ENEMY_BATTLE_SIZE &&
                      battle->enemyIds[unitIndex] != BATTLE_UNIT_LIST_END;
       unitIndex++)
    unitIds[unitIndex] = battle->enemyIds[unitIndex];
  count = unitIndex;
  BattlePlacement_LayoutEnemies(unitIds, count, xPositions, zPositions);
  for (unitIndex = 0; unitIndex < count; unitIndex++, enemyPlace++) {
    s32 unitId = battle->enemyIds[unitIndex];

    if (unitId != BATTLE_UNIT_REMOVED)
      BattleObject_Make(BattleObject_Get(unitId), unitId,
                        xPositions[enemyPlace], zPositions[enemyPlace]);
  }
}

/// Returns 1 if the unit at place is in list. Called by
/// BattleObject_SpawnObjectsForList to find the objects to release.
/// @param list unit ids, ended by BATTLE_UNIT_LIST_END, 14 at most
/// @param place 0 to 7 a party unit id, 8 to 13 the enemies
s32 BattlePlacement_ContainsId(s16 *list, s32 place) {
  u32 listIndex;

  // An enemy's place to its unit id.
  if (place > ALLY_MAX)
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
