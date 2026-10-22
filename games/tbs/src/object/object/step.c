/// @file
/// Field requests and each step of the leader: exits, encounters, damage.
// Part of object.c.

#include "../object_internal.h"

// The fields of struct BattleUnit that Field_ProcessStep reads (see its tag).
#ifndef UBFIX
/// A party member's battle unit, seen as the HP and poison a step reads.
struct StepOwner {
  u8 unitFieldsBeforeHp[0x38];
  s16 hp; ///< struct BattleUnit's hp: current HP, 0 when downed
  u8 unitFieldsBeforePoison[0x131 - 0x3a];
  u8 poison; ///< struct BattleUnit's poison: POISON_NORMAL or POISON_DEADLY
};
#endif

/// Clears the field's twelve pending requests (see struct FieldWork).
///
/// Called by Field_RunScene while it sets a scene up.
void Field_ClearRequests(void) {
  s16 noRequest;
  struct FieldWork *work;
  noRequest = 0;
  work = (struct FieldWork *)gEventWork;
  work->action = noRequest;
  work->pushTile = 0;
  work->exit = 0;
  work->menu = 0;
  work->examine = 0;
  work->start = 0;
  work->talk = 0;
  work->tile = 0;
  work->battle = 0;
  work->psynergy = 0;
  work->item = 0;
  work->event = 0;
}

/// Returns the sum of the field's twelve pending requests (counting the actor
/// event request twice): non-zero while any request is pending.
///
/// Field_RunScene tests it to stop walking and to leave its request loop.
s32 Field_CountRequests(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;

  return work->action + work->pushTile + work->exit + work->menu +
         work->examine + work->start + (s16)work->talk + work->tile +
         (s16)work->talk + work->battle + work->psynergy + work->item +
         work->event;
}

/// Returns the collision code of the map cell 16 pixels ahead of the party
/// leader (a world map block on the world map), or 0 without a leader.
///
/// Called by Field_FindItemEvent and FieldFx_FindMatchingEvent (items.c),
/// which match an item or Psynergy event to the tile the leader faces.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldWork *work;
  struct ObjectRuntime *leader;
  struct MapState *map;
  union PolarOffsetPosition position;
  u8 *tile;
  s32 fixedX;
  s32 fixedZ;

  work = WORK_SLOT(HEAP_SLOT_EVENT);
  leader = (struct ObjectRuntime *)work->actors[gGameState.selectedActor];

  map = WORK_SLOT(HEAP_SLOT_MAP);

  if (leader == 0)
    return 0;

  position.vector.x = leader->x;
  position.vector.y = leader->y;
  position.vector.z = leader->z;
  Vector_AddPolarOffset(0x100000, leader->angle, position.coordinates);

  if (work->mode == MODE_WORLD_MAP) {
    // World map blocks are 32 pixels square, 32 to a row.
    u32 tileX;
    u32 tileY;

    // Rounding toward zero, as a signed division by 0x200000 would.
    fixedX = position.vector.x;
    if (fixedX < 0)
      fixedX += 0x1fffff;
    tileX = (fixedX >> 21) & 31;

    fixedZ = position.vector.z;
    if (fixedZ < 0)
      fixedZ += 0x1fffff;
    tileY = (fixedZ >> 21) & 31;

    tile = (u8 *)gMapBlocks + ((tileX + (tileY << 5)) << 2);
  } else {
    // Map cells are 16 pixels square, 128 to a row.
    tile = (u8 *)map->layers[MAP_LAYER_GROUND].cells;

    fixedX = position.vector.x;
    if (fixedX < 0)
      fixedX += 0xfffff;
    {
      u32 tileX = fixedX >> 20;

      fixedZ = position.vector.z;
      if (fixedZ < 0)
        fixedZ += 0xfffff;

      tile = (u8 *)((u32)tile + ((tileX + ((fixedZ >> 20) << 7)) << 2));
    }
  }

  return tile[2];
}

/// Takes the exit of the first of the scene's exit regions that contains the
/// position (x, y, z), in 16.16 pixels, and whose condition holds: records the
/// region's exit value for Field_RunScene, plays the exit sound and starts
/// leaving the scene. A region's bounds are whole pixels, its maxima
/// exclusive; a table whose first minX is -1, or no table, has no exits.
///
/// Called by Field_ProcessStep when the leader steps on a cell with a non-zero
/// collision code. Sets work->exit and stops the leader.
void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ) {
  // A do/while or while scan changes register allocation (51 text bytes in
  // tbs-en); keep the sentinel backedge until that form matches.
  s32 exitValue;
  struct FieldWork *work;
  s32 fixedZ;
  s32 fixedY;
  s32 fixedX;
  s16 condition;
  s16 minZ;
  s16 maxY;
  s16 minY;
  s32 maxZ;
  s16 maxX;
  s16 minX;
  const struct SceneRegion *region;

  fixedX = positionX;
  fixedY = positionY;
  fixedZ = positionZ;
  region = gOverlayArea.regions();
  if (region != 0 && (work = (struct FieldWork *)gEventWork,
                      minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    exitValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 &&
        fixedY >= REGION_FIXED(minY) && fixedY < REGION_FIXED(maxY) &&
        fixedX >= REGION_FIXED(minX) && fixedX < REGION_FIXED(maxX) &&
        fixedZ >= REGION_FIXED(minZ) && fixedZ < REGION_FIXED(maxZ)) {
      work->exit = exitValue;
      AudioCommand_PlayFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    work = (struct FieldWork *)gEventWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

/// Handles one step of the party leader onto the cell at (x, y, z) of map
/// layer: records step events and push tiles, takes exits, and while walking
/// advances the encounter counter, regenerates PP, hurts the party with poison
/// and hazardous ground, and wears Avoid off. When this hurts the party, the
/// leader flinches and characters who fall are listed in the field work.
///
/// Called by Field_RunScene each frame while the party walks (in debug mode,
/// FLAG_DEBUG_STEPS_OFF skips it), so the counts below advance
/// per frame. Sets work->action, input, battle, command and event
/// (-1 when someone fell), work->currentTile and previousTile, walkingDistance,
/// fallenOwners and survivorCount, and gGameState's hazard and Avoid counts.
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the party read through struct StepOwner, whose alias
   * set keeps the load order and the 44-byte frame; as struct BattleUnit, 2
   * of 472 instructions differ in tbs-en. */
  struct FieldWork *work = WORK_SLOT(HEAP_SLOT_EVENT);
  struct MapState *map = WORK_SLOT(HEAP_SLOT_MAP);
  s32 leaderId = gGameState.selectedActor;
  struct ObjectRuntime *leader = (struct ObjectRuntime *)work->actors[leaderId];
  s16 previousHp[8];
  struct MapCell *tile;
#ifdef UBFIX
  // The defined form reads the struct BattleUnit UnitGetFar returns, rather
  // than through the unrelated struct StepOwner; it compiles to the same size
  // with a different register allocation (see the tag above).
  struct BattleUnit *unitVitals;
#else
  struct StepOwner *unitVitals;
#endif
  u32 partyCount;
  s32 partyHurt;
  u32 i;
  u32 event;
  s32 encounterSpeed;
  s32 stepDistance;
  s32 amount;
  s32 accumulatedDistance;
  s32 fullEncounterRate;

  partyHurt = 0;
  if (work == NULL)
    return;

  // Note each character's HP, to see who falls.
  partyCount = Party_CountActiveOwnersFar();
  for (i = 0; i < partyCount; i++)
    previousHp[i] = UnitGetFar(gGameState.activeOwners[i])->hp;

  if (work->mode == MODE_WORLD_MAP) {
    tile = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                              (((z / 0x200000) & 31) << 5)];
  } else {
    if ((u32)layer <= MAP_LAYER_OVERHEAD)
      tile = map->layers[layer].cells;
    else
      tile = (struct MapCell *)gMapCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
  }
  event = tile->collisionCode;
  work->previousTile = work->currentTile;
  work->currentTile = tile;
  // Exit regions are checked on any marked cell; the code then picks the
  // request it raises.
  if (event != 0)
    Field_CheckExitRegions(x, y, z);
  if (event >= MAP_CELL_EVENT_FIRST && event <= MAP_CELL_EVENT_LAST)
    work->action = event;
  if (event >= MAP_CELL_LEDGE && event <= MAP_CELL_JUMP_FORWARD)
    work->pushTile = event;

  // Only while walking (not climbing) towards a target.
  if (gGameState.movementMode == LEADER_MOVEMENT_WALK && leader != NULL &&
      leader->targetX != ACTOR_NO_TARGET) {
    // Encounters: the zone comes from the world map terrain, or from the
    // ground the leader stands on. FLAG_LURE_CAP doubles the speed the roll
    // counts.
    encounterSpeed = leader->speedLimit;
    if (GameFlag_TestFar(FLAG_LURE_CAP))
      encounterSpeed <<= 1;
    if (work->mode == MODE_WORLD_MAP) {
      fullEncounterRate = 1;
      work->battle = Encounter_RollWorldMap((s32)&leader->x, encounterSpeed);
    } else if (event == MAP_CELL_ENCOUNTER_GROUND_1 ||
               event == MAP_CELL_ENCOUNTER_GROUND_2) {
      fullEncounterRate =
          work->encounterGroups[event - (MAP_CELL_ENCOUNTER_GROUND_1 - 1)];
      work->battle = Encounter_RollTerrain(
          event - (MAP_CELL_ENCOUNTER_GROUND_1 - 1), encounterSpeed);
    } else {
      fullEncounterRate = work->encounterGroups[0];
      work->battle = Encounter_RollTerrain(0, encounterSpeed);
    }

    // Every 0x10000 of distance walked (counted at half speed on ground whose
    // encounter zone is 0; fullEncounterRate holds that zone, 1 on the world
    // map), the party regains PP, Djinn recover, and poison hurts.
    stepDistance = Iwram_MulQ16(work->walkingDistanceScale, leader->speedLimit);
    if (!fullEncounterRate)
      stepDistance /= 2;
    accumulatedDistance = work->walkingDistance + stepDistance;
    work->walkingDistance = accumulatedDistance;
    if (accumulatedDistance > 0xffff) {
      amount = accumulatedDistance / 0x10000;
      work->walkingDistance = accumulatedDistance & 0xffff;
      Party_AddPp(amount, 0);
      if (DjinnRecovery_TickFar())
        AudioCommand_PlayFar(SE_139);
      Event_ClearInvalidPackedValues();
      partyHurt = Party_ApplyPoisonDamage();
    }

    // Hazardous ground: entering it starts the count half way; then each
    // frame walked on it adds the leader's speed limit in whole pixels, so
    // the count follows the distance walked there (the Lamakan Desert's
    // limit is 600).
    if (gGameState.hazardMode == HAZARD_COUNT_ON_CELLS &&
        event == MAP_CELL_HAZARD) {
      if (work->previousTile->collisionCode == MAP_CELL_HAZARD)
        gGameState.hazardSteps += leader->speedLimit / 0x10000;
      else
        gGameState.hazardSteps = gGameState.hazardLimit / 2;
    }

    // Avoid wears off with distance, outside towns.
    if (gGameState.avoidDistance != 0 &&
        gGameState.sceneVariant != SCENE_VARIANT_TOWN) {
      gGameState.avoidDistance -= leader->speedLimit;
      if (gGameState.avoidDistance <= 0) {
        gGameState.avoidDistance = 1;
        // Raise Avoid as a Psynergy request that pays no PP; Avoid's end
        // handler says it wore off.
        if (work->psynergy == 0)
          work->psynergy = ACT_PACKED | ABILITY_AVOID;
      }
    }
  }

  // With HAZARD_COUNT_FRAMES, which no ☀️ code sets, the count rises by one each
  // call, that is each frame the field loop runs. Half way,
  // SPRITE_ATTACHED_EFFECT is attached to the leader until removed (0x101, at
  // sprite priority 1); at the limit it is removed (0x100) and the party takes
  // the hazard's damage.
  if (gGameState.hazardMode == HAZARD_COUNT_FRAMES) {
    gGameState.hazardSteps++;
    if (gGameState.hazardSteps == gGameState.hazardLimit / 2)
      FieldFx_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ATTACH);
    if (gGameState.hazardSteps == gGameState.hazardLimit)
      FieldFx_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_REMOVE);
  }
  if (gGameState.hazardSteps >= gGameState.hazardLimit) {
    // hazardDamage: bits 7 to 0 the amount, bit 8 a percentage of maximum HP.
    amount = gGameState.hazardDamage;
    gGameState.hazardSteps = 0;
    Party_AddHp(-(amount & 255), amount & 0x100);
    partyHurt++;
  }

  // The leader flinches (velocityY, a hop of 4 pixels a frame) and shows a
  // one-off SPRITE_ATTACHED_EFFECT; each character whose HP reached 0 on this
  // step is listed, loses its poison and raises event -1, which
  // Field_RunScene handles.
  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    leader->velocityY = 0x40000;
    FieldFx_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
    for (i = 0; i < partyCount; i++) {
#ifdef UBFIX
      unitVitals = UnitGetFar(gGameState.activeOwners[i]);
#else
      unitVitals = (struct StepOwner *)UnitGetFar(gGameState.activeOwners[i]);
#endif
      if (unitVitals->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gGameState.activeOwners[i];
        work->event = -1;
        unitVitals->poison = 0;
      }
    }
  }
}

/// Gives each character in the party amount PP (PP regenerates as the party
/// walks).
///
/// Called by Field_ProcessStep as the party walks, by Field_SearchTile for a
/// Psynergy stone (999, all PP), and by the Vale home scene through
/// Party_AddPpFar.
void Party_AddPp(s32 amount, s32 unused) {
  s32 partyCount = Party_CountActiveOwnersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    Unit_AddPpFar(gGameState.activeOwners[partyIndex], amount);
}

/// Changes each party member's HP by amount, or by that percentage of maximum
/// HP when percent is set. A percentage that truncates to zero falls back to
/// the positive magnitude of amount. Damage flashes and plays an impact sound;
/// healing plays the recovery sound.
///
/// XXX: likely the fallback was meant to keep the damage's sign, because as
/// written a damaging percentage that truncates to zero heals instead; the
/// one percentage hazard, the Lamakan Desert's 25%, truncates only below 4
/// maximum HP.
///
/// Called by Field_ProcessStep for hazardous ground.
void Party_AddHp(s32 amount, s32 percent) {
  s32 partyCount;
  s32 partyIndex;
  s32 hpDelta;
  struct BattleUnit *unit;

  // Damage of more than 10 plays the heavier impact sound.
  if (amount < 0) {
    ColorBuffer_SetSource(0x1ff, 0);
    ColorBuffer_StartInterpolation(4);
    if (amount < -10)
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
    else
      AudioCommand_PlayFar(SE_IMPACT);
  } else {
    AudioCommand_PlayFar(SE_RECOVERY);
  }

  partyCount = Party_CountActiveOwnersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    unit = UnitGetFar(gGameState.activeOwners[partyIndex]);
    if (!percent) {
      hpDelta = amount;
    } else {
      hpDelta = unit->maxHp * amount / 100;
      if (hpDelta == 0) {
        hpDelta = amount;
        if (hpDelta < 0)
          hpDelta = -hpDelta;
      }
    }
    Unit_AddHpFar(gGameState.activeOwners[partyIndex], hpDelta);
  }
}

/// Hurts each poisoned character in the party: poison takes a twentieth of
/// their maximum HP and deadly poison a tenth, rounded to the nearest and at
/// least 1.
/// Returns 0 when nobody was hurt, else the worst status found
/// (POISON_NORMAL or POISON_DEADLY), after a flash; Field_ProcessStep counts a
/// non-zero result as the party being hurt.
///
/// Called by Field_ProcessStep at each tick of the distance walked.
s32 Party_ApplyPoisonDamage(void) {
  s32 worstPoison = 0;
  s32 partyCount = Party_CountActiveOwnersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    struct BattleUnit *unit = UnitGetFar(gGameState.activeOwners[partyIndex]);
    s32 hpDelta;

    switch (unit->poison) {
    case POISON_NORMAL:
      hpDelta = -((unit->maxHp + 10) / 20);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= 0)
        worstPoison = POISON_NORMAL;
      break;
    case POISON_DEADLY:
      hpDelta = -((unit->maxHp + 5) / 10);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= POISON_NORMAL)
        worstPoison = POISON_DEADLY;
      break;
    default:
      hpDelta = 0;
      break;
    }
    Unit_AddHpFar(gGameState.activeOwners[partyIndex], hpDelta);
  }

  if (worstPoison != 0) {
    ColorBuffer_SetSource(0x1ff, 0);
    ColorBuffer_StartInterpolation(4);
    AudioCommand_PlayFar(SE_IMPACT);
  }

  return worstPoison;
}

/// Starts an event or menu on the world map: ends any colour ratio transition
/// in progress, pauses camera movement and freezes the perspective view as a
/// still picture. Elsewhere it does nothing.
///
/// Called by Field_RunScene around the handling of most requests, by the
/// Djinni effect and by the world map scene; each call is paired with
/// WorldMap_EndEvent. Changes the display transition state in
/// HEAP_SLOT_TRANSITION only when it is running. Both allocations return the
/// slot's block when it has one; with that slot free, the
/// world map takes a new, uncleared block there, kept until the scene ends,
/// whose step is then leftover memory.
void WorldMap_BeginEvent(void) {
  struct FieldWork *work;
  struct DisplayTransitionState *transition;

  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  if (work->mode == MODE_WORLD_MAP) {
    transition = Runtime_AllocateBlock(HEAP_SLOT_TRANSITION, 0x540);
    if ((transition != NULL) && (transition->step != 0)) {
      transition->start = 0;
      transition->end = 0;
      transition->duration = 1;
      transition->step = 0;
    }
    work->camera->movementState = 1;
    WorldMap_BeginStillViewFar();
  }
}

/// Ends what WorldMap_BeginEvent started: on the world map, reloads
/// the area's graphics and sets the camera's movement state back to 0.
void WorldMap_EndEvent(void) {
  struct FieldWork *work;

  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  if (work->mode == MODE_WORLD_MAP) {
    Map_LoadAreaGraphicsFar();
    work->camera->movementState = 0;
  }
}
