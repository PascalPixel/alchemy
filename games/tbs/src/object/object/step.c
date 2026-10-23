/// @file
/// Field requests and each step of the leader: warps, encounters, damage.
// Part of object.c.

#include "../object_internal.h"

// The fields of struct Unit that Field_ProcessStep reads (see its tag).
#ifndef UBFIX
/// A party member's battle unit, seen as the HP and poison a step reads.
struct StepOwner {
  u8 unitFieldsBeforeHp[0x38];
  s16 hp; ///< struct Unit's hp: current HP, 0 when downed
  u8 unitFieldsBeforePoison[0x131 - 0x3a];
  u8 poison; ///< struct Unit's poison: POISON_NORMAL or POISON_VENOM
};
#endif

/// Clears the field's twelve pending requests (see struct FieldWork).
///
/// Called by Map_Run while it sets a map up.
void Field_ClearRequests(void) {
  s16 noRequest;
  struct FieldWork *work;
  noRequest = 0;
  work = (struct FieldWork *)gFieldWork;
  work->coordEvent = noRequest;
  work->pushCell = 0;
  work->warp = 0;
  work->menu = 0;
  work->examine = 0;
  work->start = 0;
  work->talk = 0;
  work->searchCell = 0;
  work->battle = 0;
  work->psynergy = 0;
  work->item = 0;
  work->raisedEvent = 0;
}

/// Returns the sum of the field's twelve pending requests (counting the talk
/// request twice): non-zero while any request is pending.
///
/// Map_Run tests it to stop walking and to leave its request loop.
s32 Field_CountRequests(void) {
  struct FieldWork *work = (struct FieldWork *)gFieldWork;

  // BUG: a sum can be 0 with requests pending. Field_ProcessStep raises event
  // FIELD_RAISED_EVENT_PARTY_FELL (-1) on the frame a member falls to poison,
  // and the action request is the cell's metatile behavior, 1 on coord event
  // 1's cells (154 maps have them); together they sum to 0, and both wait until
  // another request breaks the tie, by which time a later cell's behavior may
  // have replaced the action. BUGFIX tests each request instead.
#ifdef BUGFIX
  return work->action | work->pushCell | work->warp | work->menu |
         work->examine | work->start | (s16)work->talk | work->tile |
         work->battle | work->psynergy | work->item | work->event;
#else
  return work->coordEvent + work->pushCell + work->warp + work->menu +
         work->examine + work->start + (s16)work->talk + work->searchCell +
         (s16)work->talk + work->battle + work->psynergy + work->item +
         work->raisedEvent;
#endif
}

/// Returns the metatile behavior of the map cell 16 pixels ahead of the party
/// leader (a world map metatile on the world map), or 0 without a leader.
///
/// Called by Field_FindItemEvent and Map_FindPsynergyEvent (items.c),
/// which match an item or Psynergy event to the tile the leader faces.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldWork *work;
  struct Object *leader;
  struct MapGridState *gridState;
  union PolarOffsetPosition position;
  u8 *tile;
  s32 fixedX;
  s32 fixedZ;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  leader = (struct Object *)work->objectEvents[gGameState.leaderLocalId];

  gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);

  if (leader == 0)
    return 0;

  position.vector.x = leader->x;
  position.vector.y = leader->y;
  position.vector.z = leader->z;
  Vector_AddPolarOffset(0x100000, leader->facing, position.coordinates);

  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    // World map metatiles are 32 pixels square, 32 to a row.
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

    tile = (u8 *)gMetatiles + ((tileX + (tileY << 5)) << 2);
  } else {
    // Map cells are 16 pixels square, 128 to a row.
    tile = (u8 *)gridState->layers[MAPGRID_LAYER_GROUND].cells;

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

/// Takes the warp of the first of the map's warp regions that contains the
/// position (x, y, z), in 16.16 pixels, and whose condition holds: records the
/// region's warp value for Map_Run, plays the warp sound and starts
/// leaving the map. A region's bounds are whole pixels, its maxima
/// exclusive; a table whose first minX is -1, or no table, has no warps.
///
/// Called by Field_ProcessStep when the leader steps on a cell with a non-zero
/// metatile behavior. Sets work->warp and stops the leader.
void Field_CheckWarpRegions(s32 positionX, s32 positionY, s32 positionZ) {
  // A do/while or while scan changes register allocation (51 text bytes in
  // tbs-en); keep the sentinel backedge until that form matches.
  s32 warpValue;
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
  const struct MapWarpRegion *region;

  fixedX = positionX;
  fixedY = positionY;
  fixedZ = positionZ;
  region = gOverlayArea.regions();
  if (region != 0 && (work = (struct FieldWork *)gFieldWork,
                      minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    warpValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 &&
        fixedY >= REGION_FIXED(minY) && fixedY < REGION_FIXED(maxY) &&
        fixedX >= REGION_FIXED(minX) && fixedX < REGION_FIXED(maxX) &&
        fixedZ >= REGION_FIXED(minZ) && fixedZ < REGION_FIXED(maxZ)) {
      work->warp = warpValue;
      Sound_PlaySongFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    work = (struct FieldWork *)gFieldWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

/// Handles one step of the party leader onto the cell at (x, y, z) of map
/// grid layer: records coord events and push tiles, takes warps, and while
/// walking advances the encounter counter, regenerates PP, hurts the party with
/// poison and hazardous ground, and wears Avoid off. When this hurts the party,
/// the leader flinches and characters who fall are listed in the field work.
///
/// Called by Map_Run each frame while the party walks (in debug mode,
/// FLAG_DEBUG_STEPS_OFF skips it), so the counts below advance
/// per frame. Sets work->action, input, battle, command and event
/// (-1 when someone fell), work->currentCell and previousCell, walkingDistance,
/// fallenOwners and survivorCount, and gGameState's hazard and Avoid counts.
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): the party read through struct StepOwner, whose alias
   * set keeps the load order and the 44-byte frame; as struct Unit, 2
   * of 472 instructions differ in tbs-en. */
  struct FieldWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD);
  struct MapGridState *gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  s32 leaderId = gGameState.leaderLocalId;
  struct Object *leader = (struct Object *)work->objectEvents[leaderId];
  s16 previousHp[8];
  struct MapGridCell *tile;
#ifdef UBFIX
  // The defined form reads the struct Unit Unit_GetFar returns, rather
  // than through the unrelated struct StepOwner; it compiles to the same size
  // with a different register allocation (see the tag above).
  struct Unit *unitVitals;
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
  partyCount = Party_CountMembersFar();
  for (i = 0; i < partyCount; i++)
    previousHp[i] = Unit_GetFar(gGameState.party[i])->hp;

  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    tile = &((struct MapGridCell *)Ram_Metatiles)[((x / 0x200000) & 31) +
                                                  (((z / 0x200000) & 31) << 5)];
  } else {
    if ((u32)layer <= MAPGRID_LAYER_OVERHEAD)
      tile = gridState->layers[layer].cells;
    else
      tile = (struct MapGridCell *)gMapGridCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
  }
  event = tile->behavior;
  work->previousCell = work->currentCell;
  work->currentCell = tile;
  // Warp regions are checked on any marked cell; the behavior then picks the
  // request it raises.
  if (event != 0)
    Field_CheckWarpRegions(x, y, z);
  if (event >= MB_EVENT_FIRST && event <= MB_EVENT_LAST)
    work->coordEvent = event;
  if (event >= MB_LEDGE && event <= MB_JUMP_FORWARD)
    work->pushCell = event;

  // Only while walking (not climbing) towards a target.
  if (gGameState.movementMode == LEADER_MOVEMENT_WALK && leader != NULL &&
      leader->targetX != OBJ_EVENT_NO_TARGET) {
    // Encounters: the zone comes from the world map terrain, or from the
    // ground the leader stands on. FLAG_LURE_CAP doubles the speed the roll
    // counts.
    encounterSpeed = leader->speedLimit;
    if (GameFlag_TestFar(FLAG_LURE_CAP))
      encounterSpeed <<= 1;
    if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
      fullEncounterRate = 1;
      work->battle = Encounter_RollWorldMap((s32)&leader->x, encounterSpeed);
    } else if (event == MB_ENCOUNTER_GROUND_1 ||
               event == MB_ENCOUNTER_GROUND_2) {
      fullEncounterRate =
          work->encounterZones[event - (MB_ENCOUNTER_GROUND_1 - 1)];
      work->battle = Encounter_RollLayoutStep(
          event - (MB_ENCOUNTER_GROUND_1 - 1), encounterSpeed);
    } else {
      fullEncounterRate = work->encounterZones[0];
      work->battle = Encounter_RollLayoutStep(0, encounterSpeed);
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
        Sound_PlaySongFar(SE_DJINNI_SET);
      FieldScript_ClearInvalidPackedValues();
      partyHurt = Party_ApplyPoisonDamage();
    }

    // Hazardous ground: entering it starts the count half way; then each
    // frame walked on it adds the leader's speed limit in whole pixels, so
    // the count follows the distance walked there (the Lamakan Desert's
    // limit is 600).
    if (gGameState.hazardMode == HAZARD_COUNT_ON_CELLS &&
        event == MB_DESERT_SAND) {
      if (work->previousCell->behavior == MB_DESERT_SAND)
        gGameState.hazardSteps += leader->speedLimit / 0x10000;
      else
        gGameState.hazardSteps = gGameState.hazardLimit / 2;
    }

    // Avoid wears off with distance, outside towns.
    if (gGameState.avoidDistance != 0 && gGameState.mapType != MAP_TYPE_TOWN) {
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
      FieldEffect_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ATTACH);
    if (gGameState.hazardSteps == gGameState.hazardLimit)
      FieldEffect_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
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
  // Map_Run handles.
  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    leader->velocityY = 0x40000;
    FieldEffect_ConfigureLinkedObject(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    for (i = 0; i < partyCount; i++) {
#ifdef UBFIX
      unitVitals = Unit_GetFar(gGameState.party[i]);
#else
      unitVitals = (struct StepOwner *)Unit_GetFar(gGameState.party[i]);
#endif
      if (unitVitals->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gGameState.party[i];
        work->raisedEvent = -1;
        unitVitals->poison = 0;
      }
    }
  }
}

/// Gives each character in the party amount PP (PP regenerates as the party
/// walks).
///
/// Called by Field_ProcessStep as the party walks, by Field_SearchCell for a
/// Psynergy stone (999, all PP), and by the Vale home map through
/// Party_AddPpFar.
void Party_AddPp(s32 amount, s32 unused) {
  s32 partyCount = Party_CountMembersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++)
    Unit_AddPpFar(gGameState.party[partyIndex], amount);
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
  struct Unit *unit;

  // Damage of more than 10 plays the heavier impact sound.
  if (amount < 0) {
    ColorBuffer_SetSource(0x1ff, 0);
    ColorBuffer_StartInterpolation(4);
    if (amount < -10)
      Sound_PlaySongFar(SE_HEAVY_IMPACT);
    else
      Sound_PlaySongFar(SE_IMPACT);
  } else {
    Sound_PlaySongFar(SE_RECOVERY);
  }

  partyCount = Party_CountMembersFar();
  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    unit = Unit_GetFar(gGameState.party[partyIndex]);
    if (!percent) {
      hpDelta = amount;
    } else {
      hpDelta = unit->hpMax * amount / 100;
      if (hpDelta == 0) {
        hpDelta = amount;
        if (hpDelta < 0)
          hpDelta = -hpDelta;
      }
    }
    Unit_AddHpFar(gGameState.party[partyIndex], hpDelta);
  }
}

/// Hurts each poisoned character in the party: poison takes a twentieth of
/// their maximum HP and deadly poison a tenth, rounded to the nearest and at
/// least 1.
/// Returns 0 when nobody was hurt, else the worst status found
/// (POISON_NORMAL or POISON_VENOM), after a flash; Field_ProcessStep counts a
/// non-zero result as the party being hurt.
///
/// Called by Field_ProcessStep at each tick of the distance walked.
s32 Party_ApplyPoisonDamage(void) {
  s32 worstPoison = 0;
  s32 partyCount = Party_CountMembersFar();
  s32 partyIndex;

  for (partyIndex = 0; partyIndex < partyCount; partyIndex++) {
    struct Unit *unit = Unit_GetFar(gGameState.party[partyIndex]);
    s32 hpDelta;

    switch (unit->poison) {
    case POISON_NORMAL:
      hpDelta = -((unit->hpMax + 10) / 20);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= 0)
        worstPoison = POISON_NORMAL;
      break;
    case POISON_VENOM:
      hpDelta = -((unit->hpMax + 5) / 10);
      if (hpDelta == 0)
        hpDelta = -1;
      if (worstPoison <= POISON_NORMAL)
        worstPoison = POISON_VENOM;
      break;
    default:
      hpDelta = 0;
      break;
    }
    Unit_AddHpFar(gGameState.party[partyIndex], hpDelta);
  }

  if (worstPoison != 0) {
    ColorBuffer_SetSource(0x1ff, 0);
    ColorBuffer_StartInterpolation(4);
    Sound_PlaySongFar(SE_IMPACT);
  }

  return worstPoison;
}

/// Starts a script or menu on the world map: ends any colour ratio transition
/// in progress, pauses camera movement and freezes the perspective view as a
/// still picture. Elsewhere it does nothing.
///
/// Called by Map_Run around the handling of most requests, by the
/// Djinni effect and by the world map's overlay; each call is paired with
/// WorldMap_EndScript. Changes the display transition state in
/// HEAP_SLOT_SCREEN_TRANSITION only when it is running. Both allocations return
/// the slot's block when it has one; with that slot free, the world map takes a
/// new, uncleared block there, kept until the map ends, whose step is then
/// leftover memory.
void WorldMap_StartScript(void) {
  struct FieldWork *work;
  struct ScreenTransitionState *state;

  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(*work));
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    state = AllocHeapSlotEwram(HEAP_SLOT_SCREEN_TRANSITION, 0x540);
    if ((state != NULL) && (state->step != 0)) {
      state->start = 0;
      state->end = 0;
      state->duration = 1;
      state->step = 0;
    }
    work->camera->movementState = 1;
    WorldMap_StartStillViewFar();
  }
}

/// Ends what WorldMap_StartScript started: on the world map, reloads
/// the area's graphics and sets the camera's movement state back to 0.
void WorldMap_EndScript(void) {
  struct FieldWork *work;

  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(*work));
  if (work->mode == OBJECT_SYSTEM_WORLD_MAP) {
    WorldMap_LoadAreaGraphicsFar();
    work->camera->movementState = 0;
  }
}
