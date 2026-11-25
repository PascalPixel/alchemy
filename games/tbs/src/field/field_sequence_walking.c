/// @file
/// Field requests and each frame of the leader's walking: exits, encounters,
/// damage.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// Clears the field's twelve pending requests (see struct FieldState).
void Field_ClearRequests(void) {
  struct FieldState *state = gFieldState;

  state->stepTrigger = 0;
  state->pushCell = 0;
  state->exit = 0;
  state->fieldMenu = 0;
  state->aButton = 0;
  state->pauseMenu = 0;
  state->talk = 0;
  state->examineCell = 0;
  state->encounter = 0;
  state->psynergy = 0;
  state->item = 0;
  state->raisedTrigger = 0;
}

/// Returns the sum of the field's twelve pending requests (counting the talk
/// request twice): non-zero while any request is pending.
s32 Field_CountRequests(void) {
  struct FieldState *state = gFieldState;

  // BUG: a sum can be 0 with requests pending. Field_UpdateWalking raises
  // FIELD_RAISED_TRIGGER_PARTY_FELL (-1) on the frame a party member falls to
  // poison, and the step trigger request is the cell's cell behavior, 1 on
  // step trigger 1's cells (154 scenes have them); together they sum to 0, and
  // both wait until another request breaks the tie, by which time a later
  // cell's behavior may have replaced the step trigger. BUGFIX tests each
  // request instead.
#ifdef BUGFIX
  return state->stepTrigger | state->pushCell | state->exit | state->fieldMenu |
         state->aButton | state->pauseMenu | (s16)state->talk |
         state->examineCell | state->encounter | state->psynergy | state->item |
         state->raisedTrigger;
#else
  return state->stepTrigger + state->pushCell + state->exit + state->fieldMenu +
         state->aButton + state->pauseMenu + (s16)state->talk +
         state->examineCell + (s16)state->talk + state->encounter +
         state->psynergy + state->item + state->raisedTrigger;
#endif
}

/// Returns the cell behavior of the map cell 16 pixels ahead of the party
/// leader (a World Map cell's tiles on the World Map), or 0 without a leader.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldState *state;
  struct Actor *leader;
  struct MapGridScrollState *gridState;
  union PolarOffsetPosition position;
  u8 *tile;
  FIXED fixedX;
  FIXED fixedZ;

  state = gFieldState;
  leader = state->actors[gMainRoutineState.leaderActorId];

  gridState = gMapGridState[0];

  if (leader == 0)
    return 0;

  position.vector.x = leader->x.fixed;
  position.vector.y = leader->y.fixed;
  position.vector.z = leader->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), leader->facing, position.coordinates);

  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    // World Map cell tiles are 32 pixels square, 32 to a row.
    u32 tileX;
    u32 tileY;

    // Rounding toward zero, as a signed division by toFIXED(32) would.
    fixedX = position.vector.x;
    /* FAKEMATCH(dead): the division by toFIXED(32) rounded towards zero by
     * hand; as (fixedX / toFIXED(32)) & 31. */
    if (fixedX < 0)
      fixedX += toFIXED(32) - 1;
    tileX = (FIXEDtoINT(fixedX) >> 5) & 31;

    fixedZ = position.vector.z;
    /* FAKEMATCH(dead): the division by toFIXED(32) rounded towards zero by
     * hand; as (fixedZ / toFIXED(32)) & 31. */
    if (fixedZ < 0)
      fixedZ += toFIXED(32) - 1;
    tileY = (FIXEDtoINT(fixedZ) >> 5) & 31;

    tile = (u8 *)gCellTiles + ((tileX + (tileY << 5)) << 2);
  } else {
    // Map cells are 16 pixels square, 128 to a row.
    tile = (u8 *)gridState->layers[MAPGRID_LAYER_GROUND].cells;

    fixedX = position.vector.x;
    {
      u32 tileX = fixedX / toFIXED(MAPGRID_CELL_PIXELS);

      fixedZ = position.vector.z;

      tile =
          (u8 *)((u32)tile +
                 ((tileX + fixedZ / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE)
                  << 2));
    }
  }

  return tile[2];
}

/// Takes the exit of the first of the scene's exit regions that contains the
/// position (x, y, z), in 16.16 pixels, and whose condition holds: records the
/// region's exit value for FieldSequence_Run, plays the exit sound and starts
/// leaving the scene.
void Field_CheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ) {
  s32 exitValue;
  struct FieldState *state;
  FIXED fixedZ;
  FIXED fixedY;
  FIXED fixedX;
  s16 condition;
  s16 minZ;
  s16 maxY;
  s16 minY;
  s32 maxZ;
  s16 maxX;
  s16 minX;
  const struct SceneExitRegion *region;

  fixedX = positionX;
  fixedY = positionY;
  fixedZ = positionZ;
  region = gOverlayArea.regions();
  if (region != 0 && (state = gFieldState, minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    exitValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 &&
        fixedY >= INTtoFIXED(minY) && fixedY < INTtoFIXED(maxY) &&
        fixedX >= INTtoFIXED(minX) && fixedX < INTtoFIXED(maxX) &&
        fixedZ >= INTtoFIXED(minZ) && fixedZ < INTtoFIXED(maxZ)) {
      state->exit = exitValue;
      Sound_PlayFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    state = gFieldState;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

/// Handles one frame of the party leader's walking onto the cell at (x, y, z)
/// of map grid layer: records step triggers and push cells, takes exits, and
/// while walking advances the encounter counter, regenerates PP, hurts the
/// party with poison and hazardous ground, and wears Avoid off.
void Field_UpdateWalking(s32 layer, FIXED x, FIXED y, FIXED z) {
  struct FieldState *state = gFieldState;
  struct MapGridScrollState *gridState = gMapGridState[0];
  s32 leaderId = gMainRoutineState.leaderActorId;
  struct Actor *leader = state->actors[leaderId];
  s16 previousHp[8];
  struct MapGridCell *tile;
  struct Unit *unitVitals;
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
  if (state == NULL)
    return;

  // Note each character's HP, to see who falls.
  partyCount = Party_CountMembersFar();
  for (i = 0; i < partyCount; i++)
    previousHp[i] = Unit_GetFar(gMainRoutineState.party[i])->hp;

  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    tile =
        &((struct MapGridCell *)Ram_CellTiles)[((x / toFIXED(32)) & 31) +
                                               (((z / toFIXED(32)) & 31) << 5)];
  } else {
    if ((u32)layer <= MAPGRID_LAYER_OVERHEAD)
      tile = gridState->layers[layer].cells;
    else
      tile = (struct MapGridCell *)gMapGridCellBuffer;
    tile = &tile[(x / toFIXED(MAPGRID_CELL_PIXELS)) +
                 (z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE];
  }
  event = tile->behavior;
  state->previousCell = state->currentCell;
  state->currentCell = tile;
  // Exit regions are checked on any marked cell; the behavior then picks the
  // request it raises.
  if (event != 0)
    Field_CheckExitRegions(x, y, z);
  if (event >= CELL_BEHAVIOR_EVENT_FIRST && event <= CELL_BEHAVIOR_EVENT_LAST)
    state->stepTrigger = event;
  if (event >= CELL_BEHAVIOR_LEDGE && event <= CELL_BEHAVIOR_JUMP_FORWARD)
    state->pushCell = event;

  // Only while walking (not climbing) towards a target.
  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_WALK &&
      leader != NULL && leader->targetX != ACTOR_NO_TARGET) {
    // Encounters: the zone comes from the World Map's encounter region and
    // battle environment, or from the ground the leader stands on.
    encounterSpeed = leader->speedLimit;
    if (GameFlag_GetFar(FLAG_LURE_CAP))
      encounterSpeed <<= 1;
    if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
      fullEncounterRate = 1;
      state->encounter =
          Encounter_RollWorldMap((s32)&leader->x.fixed, encounterSpeed);
    } else if (event == CELL_BEHAVIOR_ENCOUNTER_GROUND_A ||
               event == CELL_BEHAVIOR_ENCOUNTER_GROUND_B) {
      fullEncounterRate =
          state->encounterZones[event - (CELL_BEHAVIOR_ENCOUNTER_GROUND_A - 1)];
      state->encounter = Encounter_RollMapStep(
          event - (CELL_BEHAVIOR_ENCOUNTER_GROUND_A - 1), encounterSpeed);
    } else {
      fullEncounterRate = state->encounterZones[0];
      state->encounter = Encounter_RollMapStep(0, encounterSpeed);
    }

    // Every FIXED_ONE of distance walked (counted at half speed on ground whose
    // encounter zone is 0; fullEncounterRate holds that zone, 1 on the World
    // Map), the party regains PP, Djinn recover, and poison hurts.
    stepDistance = Iwram_MulFX(state->walkingDistanceScale, leader->speedLimit);
    if (!fullEncounterRate)
      stepDistance /= 2;
    accumulatedDistance = state->walkingDistance + stepDistance;
    state->walkingDistance = accumulatedDistance;
    if (accumulatedDistance >= FIXED_ONE) {
      amount = accumulatedDistance / FIXED_ONE;
      state->walkingDistance = accumulatedDistance & (FIXED_ONE - 1);
      Party_AddPp(amount, 0);
      if (DjinnRecovery_CountDownFar())
        Sound_PlayFar(SE_DJINNI_SET);
      Party_ClearUnusableShortcuts();
      partyHurt = Party_ApplyPoisonDamage();
    }

    // Hazardous ground: entering it starts the count half way; then each
    // frame walked on it adds the leader's speed limit in whole pixels, so
    // the count follows the distance walked there (the Lamakan Desert's
    // limit is 600).
    if (gMainRoutineState.hazardMode == HAZARD_COUNT_ON_CELLS &&
        event == CELL_BEHAVIOR_DESERT_SAND) {
      if (state->previousCell->behavior == CELL_BEHAVIOR_DESERT_SAND)
        gMainRoutineState.hazardSteps += leader->speedLimit / FIXED_ONE;
      else
        gMainRoutineState.hazardSteps = gMainRoutineState.hazardLimit / 2;
    }

    // Avoid wears off with distance, outside towns.
    if (gMainRoutineState.avoidDistance != 0 &&
        gMainRoutineState.sceneType != SCENE_TYPE_TOWN) {
      gMainRoutineState.avoidDistance -= leader->speedLimit;
      if (gMainRoutineState.avoidDistance <= 0) {
        gMainRoutineState.avoidDistance = 1;
        // Raise Avoid as a Psynergy request that pays no PP; Avoid's end
        // handler says it wore off.
        if (state->psynergy == 0)
          state->psynergy = ACT_PACKED | ABILITY_AVOID;
      }
    }
  }

  // With HAZARD_COUNT_FRAMES, which nothing in the game sets, the count rises
  // by one each call, that is each frame the field sequence runs.
  if (gMainRoutineState.hazardMode == HAZARD_COUNT_FRAMES) {
    gMainRoutineState.hazardSteps++;
    if (gMainRoutineState.hazardSteps == gMainRoutineState.hazardLimit / 2)
      FieldEffect_SetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                            ATTACHED_EFFECT_ATTACH);
    if (gMainRoutineState.hazardSteps == gMainRoutineState.hazardLimit)
      FieldEffect_SetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                            ATTACHED_EFFECT_REMOVE);
  }
  if (gMainRoutineState.hazardSteps >= gMainRoutineState.hazardLimit) {
    // hazardDamage: the amount below HAZARD_DAMAGE_PERCENT, which makes it a
    // percentage of maximum HP.
    amount = gMainRoutineState.hazardDamage;
    gMainRoutineState.hazardSteps = 0;
    Party_AddHp(-(amount & (HAZARD_DAMAGE_PERCENT - 1)),
                amount & HAZARD_DAMAGE_PERCENT);
    partyHurt++;
  }

  // The leader flinches (velocityY, a hop of 4 pixels a frame) and shows a
  // one-off SPRITE_ATTACHED_EFFECT; each character whose HP reached 0 on this
  // frame is listed, loses its poison and raises
  // FIELD_RAISED_TRIGGER_PARTY_FELL, which FieldSequence_Run handles.
  if (partyHurt) {
    state->fallenCount = 0;
    state->survivorCount = 0;
    leader->velocityY = toFIXED(4);
    FieldEffect_SetAttached(leaderId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    for (i = 0; i < partyCount; i++) {
      unitVitals = Unit_GetFar(gMainRoutineState.party[i]);
      if (unitVitals->hp > 0) {
        state->survivorCount++;
      } else if (previousHp[i] != 0) {
        state->fallenOwners[state->fallenCount++] = gMainRoutineState.party[i];
        state->raisedTrigger = FIELD_RAISED_TRIGGER_PARTY_FELL;
        unitVitals->poison = 0;
      }
    }
  }
}

/// Gives each character in the party amount PP (PP regenerates as the party
/// walks).
void Party_AddPp(s32 amount, s32 unused) {
  s32 partyCount = Party_CountMembersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    Unit_AddPpFar(gMainRoutineState.party[partyIndex], amount);
}

/// Damage past this plays SE_HEAVY_IMPACT rather than SE_IMPACT.
#define HEAVY_DAMAGE 10

/// Changes each party member's HP by amount, or by that percentage of maximum
/// HP when percent is set.
void Party_AddHp(s32 amount, s32 percent) {
  s32 partyCount;
  s32 partyIndex;
  s32 hpDelta;
  struct Unit *unit;

  if (amount < 0) {
    // The screen flashes orange.
    PaletteFade_SetSource(RGB(31, 15, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(4);
    if (amount < -HEAVY_DAMAGE)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    else
      Sound_PlayFar(SE_IMPACT);
  } else {
    Sound_PlayFar(SE_RECOVERY);
  }

  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    unit = Unit_GetFar(gMainRoutineState.party[partyIndex]);
    if (!percent) {
      hpDelta = amount;
    } else {
      hpDelta = unit->hpMax * amount / 100;
      // The fallback drops the damage's sign, so a damaging percentage that
      // truncates to zero heals instead.
      if (hpDelta == 0) {
        hpDelta = amount;
        if (hpDelta < 0)
          hpDelta = -hpDelta;
      }
    }
    Unit_AddHpFar(gMainRoutineState.party[partyIndex], hpDelta);
  }
}

/// The share of maximum HP poison and deadly poison take each time, as one part
/// in this many.
#define POISON_HP_PARTS 20
#define VENOM_HP_PARTS 10

/// Hurts each poisoned character in the party: poison takes a twentieth of
/// their maximum HP and deadly poison a tenth, rounded to the nearest and at
/// least 1.
s32 Party_ApplyPoisonDamage(void) {
  s32 worstPoison = 0;
  s32 partyCount = Party_CountMembersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    struct Unit *unit = Unit_GetFar(gMainRoutineState.party[partyIndex]);
    s32 hpDelta;

    switch (unit->poison) {
    case POISON_NORMAL:
      hpDelta = -((unit->hpMax + POISON_HP_PARTS / 2) / POISON_HP_PARTS);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= 0)
        worstPoison = POISON_NORMAL;
      break;
    case POISON_VENOM:
      hpDelta = -((unit->hpMax + VENOM_HP_PARTS / 2) / VENOM_HP_PARTS);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= POISON_NORMAL)
        worstPoison = POISON_VENOM;
      break;
    default:
      hpDelta = 0;
      break;
    }
    Unit_AddHpFar(gMainRoutineState.party[partyIndex], hpDelta);
  }

  if (worstPoison != 0) {
    PaletteFade_SetSource(RGB(31, 15, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(4);
    Sound_PlayFar(SE_IMPACT);
  }

  return worstPoison;
}

/// Before an event or menu on the World Map: ends any screen transition in
/// progress, stops the camera and holds the view as a still image.
void WorldMap_Freeze(void) {
  struct FieldState *fieldState;
  struct ScreenTransitionState *state;

  fieldState = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(*fieldState));
  if (fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
    state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION,
                                sizeof(struct ScreenTransitionState));
    if ((state != NULL) && (state->step != 0)) {
      state->start = 0;
      state->end = 0;
      state->duration = 1;
      state->step = 0;
    }
    fieldState->camera->paused = 1;
    WorldMap_StartStillImageFar();
  }
}

/// Ends what WorldMap_Freeze started: on the World Map, reloads the area's
/// graphics and lets the camera move again.
void WorldMap_Unfreeze(void) {
  struct FieldState *state;

  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(*state));
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    WorldMap_LoadAreaGraphicsFar();
    state->camera->paused = 0;
  }
}

#undef HEAVY_DAMAGE
#undef POISON_HP_PARTS
#undef VENOM_HP_PARTS
