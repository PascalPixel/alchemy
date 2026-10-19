// Part of object.c, the field object, compiled only through it.
// Field requests and each step of the leader: exits, encounters, damage.

#include "../object_internal.h"

// The fields of struct BattleUnit that Field_ProcessStep reads (see its tag).
#ifndef UBFIX
struct StepOwner {
  u8 unitFieldsBeforeHp[0x38];
  s16 hp;
  u8 unitFieldsBeforePoison[0x131 - 0x3a];
  u8 poison;
};
#endif

// The collision codes of map cells that mean something when stepped on.
enum StepTileEvent {
  STEP_EVENT_FIRST = 1, // 1 to 239: the scene's step events
  STEP_EVENT_LAST = 239,
  STEP_TERRAIN_FIRST = 240, // 240 and 241: ground with its own encounters
  STEP_TERRAIN_LAST = 241,
  STEP_EVENT_HAZARD = 250, // ground that hurts the party over time
  STEP_WARP_FIRST = 252,   // 252 to 254: warps
  STEP_WARP_LAST = 254
};

// Clears the field's twelve pending requests (see struct FieldWork).
void Field_ClearRequests(void) {
  s16 noRequest;
  struct FieldWork *work;
  noRequest = 0;
  work = (struct FieldWork *)gEventWork;
  work->action = noRequest;
  work->input = 0;
  work->exit = 0;
  work->menu = 0;
  work->examine = 0;
  work->start = 0;
  work->descriptor = 0;
  work->tile = 0;
  work->battle = 0;
  work->command = 0;
  work->item = 0;
  work->event = 0;
}

// Returns the sum of the field's twelve pending requests (counting the actor
// event request twice): non-zero while any request is pending.
s32 Field_CountRequests(void) {
  struct FieldWork *work = (struct FieldWork *)gEventWork;

  return work->action + work->input + work->exit + work->menu + work->examine +
         work->start + (s16)work->descriptor + work->tile +
         (s16)work->descriptor + work->battle + work->command + work->item +
         work->event;
}

// Returns the collision code of the map cell 16 pixels ahead of the party
// leader (a world map block on the world map), or 0 without a leader.
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

  map = WORK_SLOT(8);

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
    tile = (u8 *)map->layers[0].cells;

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

// Takes the exit of the first of the scene's exit regions that contains the
// position (x, y, z) and whose condition holds: records the region's exit
// value for Field_RunScene, plays the exit sound and starts leaving the scene.
// A do/while or while scan changes register allocation (51 text bytes in
// tbs-en, 2026-10-16); keep the sentinel backedge until that form matches.
void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ) {
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

// Handles one step of the party leader onto the cell at (x, y, z) of map
// layer: records step events and warps, takes exits, and while walking
// advances the encounter counter, regenerates PP, hurts the party with poison
// and hazardous ground, and wears Avoid off. When this hurts the party, the
// leader flinches and characters who fall are listed in the field work.
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the party read through struct StepOwner, the prefix
   * of struct BattleUnit this reads, whose own alias set keeps the party-load
   * order and the 44-byte frame; as struct BattleUnit, 2 of 472 instructions
   * differ in tbs-en. */
  struct FieldWork *work = WORK_SLOT(HEAP_SLOT_EVENT);
  struct MapState *map = WORK_SLOT(8);
  s32 leaderId = gGameState.selectedActor;
  struct ObjectRuntime *leader = (struct ObjectRuntime *)work->actors[leaderId];
  s16 previousHp[8];
  struct MapCell *tile;
#ifdef UBFIX
  // UnitGet returns the real BattleUnit; the native prefix aliases that owner
  // through an unrelated struct type. The plain owner keeps 0x41c bytes but
  // changes allocation from .text+0x1c90 in English (2026-10-16).
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
    if ((u32)layer <= 2)
      tile = map->layers[layer].cells;
    else
      tile = (struct MapCell *)gMapCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
  }
  event = tile->collisionCode;
  work->previousTile = work->currentTile;
  work->currentTile = tile;
  if (event != 0)
    Field_CheckExitRegions(x, y, z);
  if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
    work->action = event;
  if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
    work->input = event;

  if (gGameState.movementMode == 0 && leader != NULL &&
      leader->targetX != ACTOR_NO_TARGET) {
    // Encounters: the zone comes from the world map terrain, or from the
    // ground the leader stands on.
    encounterSpeed = leader->speedLimit;
    if (GameFlag_TestFar(FLAG_LURE_CAP))
      encounterSpeed <<= 1;
    if (work->mode == MODE_WORLD_MAP) {
      fullEncounterRate = 1;
      work->battle = Encounter_RollWorldMap((s32)&leader->x, encounterSpeed);
    } else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST) {
      fullEncounterRate =
          work->encounterGroups[event - (STEP_TERRAIN_FIRST - 1)];
      work->battle = Encounter_RollTerrain(event - (STEP_TERRAIN_FIRST - 1),
                                           encounterSpeed);
    } else {
      fullEncounterRate = work->encounterGroups[0];
      work->battle = Encounter_RollTerrain(0, encounterSpeed);
    }

    // Every 0x10000 of distance walked (half as fast where full is 0), the
    // party regains PP and takes poison damage.
    stepDistance = Iwram_MulQ16(work->walkingDistanceScale, leader->speedLimit);
    if (!fullEncounterRate)
      stepDistance /= 2;
    accumulatedDistance = work->walkingDistance + stepDistance;
    work->walkingDistance = accumulatedDistance;
    if (accumulatedDistance > 0xffff) {
      amount = accumulatedDistance / 0x10000;
      work->walkingDistance = accumulatedDistance & 0xffff;
      BattleParty_ApplyDrain(amount, 0);
      if (BattlePlacement_UpdateTimedEntriesFar())
        AudioCommand_PlayFar(SE_139);
      Event_ClearInvalidPackedValues();
      partyHurt = BattleParty_ApplyStatusDamage();
    }

    // Hazardous ground: entering it starts the count half way.
    if (gGameState.hazardMode == 0 && event == STEP_EVENT_HAZARD) {
      if (work->previousTile->collisionCode == STEP_EVENT_HAZARD)
        gGameState.hazardSteps += leader->speedLimit / 0x10000;
      else
        gGameState.hazardSteps = gGameState.hazardLimit / 2;
    }

    // Avoid wears off with distance, outside scenes of variant 2.
    if (gGameState.avoidDistance != 0 && gGameState.sceneVariant != 2) {
      gGameState.avoidDistance -= leader->speedLimit;
      if (gGameState.avoidDistance <= 0) {
        gGameState.avoidDistance = 1;
        if (work->command == 0)
          work->command = 0x2096;
      }
    }
  }

  // In hazard mode 1 the count rises each step, with warnings half way and at
  // the limit; at the limit the party takes the hazard's damage.
  if (gGameState.hazardMode == 1) {
    gGameState.hazardSteps++;
    if (gGameState.hazardSteps == gGameState.hazardLimit / 2)
      BattleFx_ConfigureLinkedObject(leaderId, 0x101);
    if (gGameState.hazardSteps == gGameState.hazardLimit)
      BattleFx_ConfigureLinkedObject(leaderId, 0x100);
  }
  if (gGameState.hazardSteps >= gGameState.hazardLimit) {
    amount = gGameState.hazardDamage;
    gGameState.hazardSteps = 0;
    BattleParty_ApplyHealthDelta(-(amount & 255), amount & 0x100);
    partyHurt++;
  }

  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    leader->velocityY = 0x40000;
    BattleFx_ConfigureLinkedObject(leaderId, 0x102);
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

// Gives each character in the party amount PP (PP regenerates as the party
// walks).
void BattleParty_ApplyDrain(s32 amount, s32 unused) {
  s32 partyCount = Party_CountActiveOwnersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    Unit_AddPpFar(gGameState.activeOwners[partyIndex], amount);
}

// Changes each party member's HP by amount, or by that percentage of maximum
// HP when percent is set. A percentage that truncates to zero falls back to
// the positive magnitude of amount. Damage flashes and plays an impact sound;
// healing plays the recovery sound.
void BattleParty_ApplyHealthDelta(s32 amount, s32 percent) {
  s32 partyCount;
  s32 partyIndex;
  s32 hpDelta;
  struct BattleUnit *unit;

  if (amount < 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    if (amount < -10)
      AudioCommand_PlayFar(SE_HEAVY_IMPACT);
    else
      AudioCommand_PlayFar(SE_133);
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

// Hurts each poisoned character in the party: poison takes a twentieth of
// their maximum HP and venom a tenth (at least 1). Returns 0 when nobody was
// hurt, else the worst status found (1 poison, 2 venom), after a flash.
s32 BattleParty_ApplyStatusDamage(void) {
  s32 worstPoison = 0;
  s32 partyCount = Party_CountActiveOwnersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    struct BattleUnit *unit = UnitGetFar(gGameState.activeOwners[partyIndex]);
    s32 hpDelta;

    switch (unit->poison) {
    case 1:
      hpDelta = -((unit->maxHp + 10) / 20);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= 0)
        worstPoison = 1;
      break;
    case 2:
      hpDelta = -((unit->maxHp + 5) / 10);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= 1)
        worstPoison = 2;
      break;
    default:
      hpDelta = 0;
      break;
    }
    Unit_AddHpFar(gGameState.activeOwners[partyIndex], hpDelta);
  }

  if (worstPoison != 0) {
    BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
    BattleFx_StartBufferInterpolation(4);
    AudioCommand_PlayFar(SE_133);
  }

  return worstPoison;
}

// Starts an event or menu on the world map: ends any colour ratio transition
// in progress, sets the camera's movement state to 1 and resumes the map's
// animation. Elsewhere it does nothing.
void Battle_SetObjectFlag5bWhenMode3(void) {
  struct FieldWork *work;
  struct DisplayTransitionState *transition;

  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  if (work->mode == MODE_WORLD_MAP) {
    transition = Runtime_AllocateBlock(0x1F, 0x540);
    if ((transition != NULL) && (transition->step != 0)) {
      transition->start = 0;
      transition->end = 0;
      transition->duration = 1;
      transition->step = 0;
    }
    work->camera->movementState = 1;
    Map_ResumeAnimationFar();
  }
}

// Ends what Battle_SetObjectFlag5bWhenMode3 started: on the world map, reloads
// the area's graphics and sets the camera's movement state back to 0.
void Battle_ClearObjectFlag5bWhenMode3(void) {
  struct FieldWork *work;

  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  if (work->mode == MODE_WORLD_MAP) {
    Map_LoadAreaGraphicsFar();
    work->camera->movementState = 0;
  }
}
