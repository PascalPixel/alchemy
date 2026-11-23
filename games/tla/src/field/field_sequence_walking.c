/// @file
/// Field requests and each frame of the leader's walking: exits, encounters,
/// damage.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/emote.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/game.h"
#include "system/ram_buffer.h"
#include "macros.h"

/// MainRoutineWork.hazardDamage's amount, below HAZARD_DAMAGE_PERCENT.
#define HAZARD_DAMAGE_AMOUNT 0xff
/// A loss of more HP than this plays SE_HEAVY_IMPACT rather than SE_IMPACT.
#define HEAVY_DAMAGE 10
/// Poison takes 1/POISON_DAMAGE_DIVISOR of a character's maximum HP, rounded to
/// the nearest, and venom ("deadly poison") 1/VENOM_DAMAGE_DIVISOR.
#define POISON_DAMAGE_DIVISOR 20
#define VENOM_DAMAGE_DIVISOR 10

/// Unused: returns 0.
UNUSED s32 FieldSequence_ReturnZero(void) { return 0; }

/// Clears the field's twelve pending requests (see struct FieldWork).
void Field_ClearRequests(void) {
  struct FieldWork *work = gFieldWork;

  work->stepTrigger = 0;
  work->pushCell = 0;
  work->exit = 0;
  work->fieldMenu = 0;
  work->aButton = 0;
  work->pauseMenu = 0;
  work->talk = 0;
  work->examineCell = 0;
  work->encounter = 0;
  work->psynergy = 0;
  work->item = 0;
  work->raisedTrigger = 0;
}

/// Returns the sum of the field's twelve pending requests: non-zero while any
/// request is pending.
s32 Field_CountRequests(void) {
  struct FieldWork *work = gFieldWork;

  // BUG: a sum can be 0 with requests pending. Field_UpdateWalking raises
  // FIELD_RAISED_TRIGGER_PARTY_FELL (-1) on the frame a party member falls to
  // poison, and the step trigger request is the cell's cell behavior, 1 on
  // step trigger 1's cells; together they sum to 0, and both wait until
  // another request breaks the tie. BUGFIX tests each request instead.
#ifdef BUGFIX
  return work->stepTrigger | work->pushCell | work->exit | work->fieldMenu |
         work->aButton | work->pauseMenu | (s16)work->talk | work->examineCell |
         work->encounter | work->psynergy | work->item | work->raisedTrigger;
#else
  return work->stepTrigger + work->pushCell + work->exit + work->fieldMenu +
         work->aButton + work->pauseMenu + work->talk + work->examineCell +
         work->encounter + work->psynergy + work->item + work->raisedTrigger;
#endif
}

/// Sets the field work's hoverReady when the ship flies (FLAG_SHIP_FLIES) and a
/// party member knows Hover; clears it otherwise. The world map's pad movement
/// reads it.
void Field_UpdateHoverReady(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (GameFlag_Get(FLAG_SHIP_FLIES) &&
      Party_FindOwnerWithActionFar(ABILITY_HOVER) != -1)
    work->hoverReady = 1;
  else
    work->hoverReady = 0;
}

/// Returns the cell behavior of the map cell 16 pixels ahead of the party
/// leader (on the World Map, of its 32-pixel cell), or 0 without a leader.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Field_GetPartyActor();
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  FIXED position[3];
  u8 *cell;

  if (actor == NULL)
    return 0;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), actor->facing, position);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 x, z;

    x = position[0];
    z = position[2];
    cell = Ram_MapBlocks +
           ((((x / toFIXED(32)) & 31) + (((z / toFIXED(32)) & 31) << 5)) << 2);
  } else {
    FIXED x, z;

    x = position[0];
    cell = (u8 *)map->layers[0].cells;
    z = position[2];
    cell += (x / toFIXED(MAPGRID_CELL_PIXELS) +
                 z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE
             << 2);
  }
  return cell[2];
}

/// The collision code of the cell under the leader (on the world map, of its
/// block), or 0 with no leader. Teleport's triggers match it.
s32 Field_GetCollisionUnderLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Field_GetPartyActor();
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Vector3 position;
  struct MapCell *cell;

  if (actor == NULL)
    return 0;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    cell = (struct MapCell *)Ram_MapBlocks +
           (((position.x / toFIXED(32)) & 31) +
            ((position.z / toFIXED(32)) & 31) * 32);
  } else {
    cell = map->layers[0].cells;
    cell += position.x / toFIXED(MAPGRID_CELL_PIXELS) +
            position.z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  }
  return cell->collisionCode;
}

/// Takes the exit of the first of the scene's exit regions that contains the
/// position (x, y, z), in 16.16 pixels, and whose condition holds: records the
/// region's exit value for FieldSequence_Run, plays the exit sound and starts
/// leaving the scene.
void Field_CheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ) {
  s32 selectedValue;
  struct FieldWork *work;
  FIXED z;
  FIXED y;
  FIXED x;
  s16 condition;
  s16 minZ;
  s16 maxY;
  s16 minY;
  s32 maxZ;
  s16 maxX;
  s16 minX;
  struct MapRegion *region;

  x = positionX;
  y = positionY;
  z = positionZ;
  region = (struct MapRegion *)SCENE_ENTRIES->regions();
  if (region != 0 &&
      (work = Ram_HeapSlots->fieldWork, minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    selectedValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 && y >= INTtoFIXED(minY) &&
        y < INTtoFIXED(maxY) && x >= INTtoFIXED(minX) && x < INTtoFIXED(maxX) &&
        z >= INTtoFIXED(minZ) && z < INTtoFIXED(maxZ)) {
      work->exit = (u16)selectedValue;
      Sound_PlayFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    work = Ram_HeapSlots->fieldWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

/// Brings the party's recovering Djinn one turn nearer, setting each that is
/// ready, and returns 1 when any was set.
s32 DjinnRecovery_CountDownFar(void);
void FieldEffect_SetAttached(s32 owner, s32 mode);

/// Handles one frame of the party leader's walking onto the cell at (x, y, z)
/// of map grid layer: records step triggers and push cells, takes exits, and
/// while walking advances the encounter counter, regenerates PP, hurts the
/// party with poison and hazardous ground, and wears Avoid off.
void Field_UpdateWalking(u32 layer, FIXED x, FIXED y, FIXED z) {
  struct FieldWork *work = gFieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 leaderId = Party_GetLeaderActor();
  struct Actor *leader = Field_GetPartyActor();
  s16 previousHp[8];
  struct MapCell *tile;
  struct Unit *unit;
  s32 partyHurt;
  u32 partyCount;
  u32 i;
  u32 event;
  u32 attributes;
  s32 speed;
  s32 step;
  s32 amount;
  s32 distance;
  s32 zone;

  partyHurt = 0;
  if (work == NULL)
    return;

  partyCount = Party_CountMembersFar();
  for (i = 0; i < partyCount; i++)
    previousHp[i] = Unit_Get(gMainRoutineWork.party[i])->hp;

  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    tile = &((struct MapCell *)Ram_MapBlocks)[((x / toFIXED(32)) & 31) +
                                              (((z / toFIXED(32)) & 31) << 5)];
    event = tile->collisionCode;
    attributes = 0;
  } else {
    if (layer <= 2)
      tile = map->layers[layer].cells;
    else
      tile = (struct MapCell *)Ram_MapCellBuffer;
    tile = &tile[x / toFIXED(MAPGRID_CELL_PIXELS) +
                 z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE];
    event = tile->collisionCode;
    attributes = tile->attributeB;
  }
  work->previousCell = work->currentCell;
  work->currentCell = tile;
  if (attributes != 0)
    Field_CheckExitRegions(x, y, z);
  if (event - CELL_BEHAVIOR_EVENT_FIRST <=
      CELL_BEHAVIOR_EVENT_LAST - CELL_BEHAVIOR_EVENT_FIRST)
    work->stepTrigger = event;
  if (attributes &
      (MAP_CELL_PUSH | MAP_CELL_LEDGE | MAP_CELL_CLIMBABLE | MAP_CELL_JUMP))
    work->pushCell = attributes;

  if ((gMainRoutineWork.movementMode <= LEADER_MOVEMENT_WORLD_MAP ||
       gMainRoutineWork.movementMode == LEADER_MOVEMENT_SHIP_ON_MAP ||
       gMainRoutineWork.movementMode == LEADER_MOVEMENT_SHIP) &&
      leader != NULL && leader->targetX != ACTOR_NO_TARGET) {
    zone = 0;
    speed = leader->speedLimit;
    if (GameFlag_Get(FLAG_LURE_CAP))
      speed <<= 1;
    if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
      work->encounter = Encounter_RollWorldMap((s32)&leader->x.fixed, speed);
    } else {
      if (attributes & MAP_CELL_SECOND_ZONE)
        zone = 1;
      work->encounter = Encounter_RollMapStep(zone, speed);
    }

    // Every FIXED_ONE of distance walked (counted at half speed on ground whose
    // encounter zone is 0), the party regains PP, Djinn recover, and poison
    // hurts.
    step = Iwram_MulFX(work->walkingDistanceScale, leader->speedLimit);
    if (!zone)
      step /= 2;
    distance = work->walkingDistance + step;
    work->walkingDistance = distance;
    if (distance >= FIXED_ONE) {
      amount = distance / FIXED_ONE;
      work->walkingDistance = distance & (FIXED_ONE - 1);
      Party_AddPp(amount, 0);
      if (DjinnRecovery_CountDownFar())
        Sound_PlayFar(SE_DJINNI_SET);
      Party_ClearUnusableShortcuts();
      partyHurt = Party_ApplyPoisonDamage();
    }

    if (gMainRoutineWork.hazardMode == HAZARD_COUNT_ON_CELLS &&
        (attributes & MAP_CELL_HAZARD)) {
      if (work->previousCell->attributeB & MAP_CELL_HAZARD)
        gMainRoutineWork.hazardSteps += leader->speedLimit / FIXED_ONE;
      else
        gMainRoutineWork.hazardSteps = gMainRoutineWork.hazardLimit / 2;
    }

    if (gMainRoutineWork.avoidDistance != 0 &&
        gMainRoutineWork.sceneType != SCENE_TYPE_TOWN) {
      gMainRoutineWork.avoidDistance -= leader->speedLimit;
      if (gMainRoutineWork.avoidDistance <= 0) {
        gMainRoutineWork.avoidDistance = 1;
        if (work->psynergy == 0)
          work->psynergy = ACT_PACKED | ABILITY_AVOID;
      }
    }
  }

  // With HAZARD_COUNT_FRAMES, which nothing in the game sets, the count rises
  // by one each call, that is each frame the field sequence runs.
  if (gMainRoutineWork.hazardMode == HAZARD_COUNT_FRAMES) {
    gMainRoutineWork.hazardSteps++;
    if ((s16)gMainRoutineWork.hazardSteps == gMainRoutineWork.hazardLimit / 2)
      FieldEffect_SetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                            ATTACHED_EFFECT_ATTACH);
    if ((s16)gMainRoutineWork.hazardSteps == gMainRoutineWork.hazardLimit)
      FieldEffect_SetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
                                            ATTACHED_EFFECT_REMOVE);
  }
  if ((s16)gMainRoutineWork.hazardSteps >= gMainRoutineWork.hazardLimit) {
    amount = gMainRoutineWork.hazardDamage;
    gMainRoutineWork.hazardSteps = 0;
    Party_AddHp(-(amount & HAZARD_DAMAGE_AMOUNT),
                amount & HAZARD_DAMAGE_PERCENT);
    partyHurt++;
  }

  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    FieldEffect_SetAttached(leaderId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    for (i = 0; i < partyCount; i++) {
      unit = Unit_Get(gMainRoutineWork.party[i]);
      if (unit->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gMainRoutineWork.party[i];
        work->raisedTrigger = FIELD_RAISED_TRIGGER_PARTY_FELL;
        unit->poison = 0;
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
    Unit_AddPpFar(gMainRoutineWork.party[partyIndex], amount);
}

/// Changes each party member's HP by amount, or by that percentage of maximum
/// HP when scaled is set; a loss flashes the screen orange.
void Party_AddHp(s32 amount, s32 scaled) {
  s32 count;
  s32 i;
  s32 value;
  struct Unit *unit;
  if (amount < 0) {
    PaletteFade_SetSource(RGB(31, 15, 0), 0);
    PaletteFade_StartInterpolation(4);
    if (amount < -HEAVY_DAMAGE)
      Sound_PlayFar(SE_HEAVY_IMPACT);
    else
      Sound_PlayFar(SE_IMPACT);
  } else {
    Sound_PlayFar(SE_RECOVERY);
  }
  count = Party_CountMembersFar();
  for (i = 0; i < count; i++) {
    unit = Unit_Get(gMainRoutineWork.party[i]);
    if (!scaled) {
      value = amount;
    } else {
      value = unit->hpMax * amount / 100;
      // XXX: a percentage that comes to 0 changes HP by amount's size as a
      // gain, a loss included; a hazard of 1% on a character under 100
      // maximum HP, watched, would show whether the game heals there.
      if (value == 0) {
        value = amount;
        if (value < 0)
          value = -value;
      }
    }
    Unit_AddHpFar(gMainRoutineWork.party[i], value);
  }
}

/// Hurts each poisoned character in the party: poison takes a twentieth of
/// their maximum HP and deadly poison a tenth, rounded to the nearest and at
/// least 1. Returns the worst poison found: 0 none, 1 poison, 2 deadly poison.
s32 Party_ApplyPoisonDamage(void) {
  s32 result = 0;
  s32 count = Party_CountMembersFar();

  if (result < count) {
    struct MainRoutineWork *work = &gMainRoutineWork;
    u8 *entry = work->party;
    s32 remaining = count;

    do {
      struct Unit *unit = Unit_Get(*entry);
      s32 amount;

      switch (unit->poison) {
      case POISON_NORMAL:
        amount = -((unit->hpMax + POISON_DAMAGE_DIVISOR / 2) /
                   POISON_DAMAGE_DIVISOR);
        if (amount == 0)
          amount = -1;
        if (result <= 0)
          result = 1;
        break;
      case POISON_VENOM:
        amount =
            -((unit->hpMax + VENOM_DAMAGE_DIVISOR / 2) / VENOM_DAMAGE_DIVISOR);
        if (amount == 0)
          amount = -1;
        if (result <= 1)
          result = 2;
        break;
      default:
        amount = 0;
        break;
      }

      remaining--;
      Unit_AddHpFar(*entry, amount);
      entry++;
    } while (remaining != 0);
  }

  if (result != 0) {
    PaletteFade_SetSource(RGB(31, 15, 0), 0);
    PaletteFade_StartInterpolation(4);
    Sound_PlayFar(SE_IMPACT);
  }

  return result;
}

/// On the World Map, with no area graphics already pending: marks them
/// pending, pauses the camera's actor and, when a screen transition is
/// running, ends it at once and waits 2 frames; then shows the still picture
/// (WorldMap_StartStillPictureFar).
void WorldMap_Freeze(void) {
  struct FieldWork *work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(*work));

  // The mode and areaGraphicsPending read as one halfword.
  if (*(u16 *)&work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    struct ScreenTransitionState *state;

    work->areaGraphicsPending = 1;

    work->camera->paused = 1;
    state = HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
    if (state != NULL && state->step != 0) {
      state->start = DISPLAY_HEIGHT / 2;
      state->end = 0;
      state->duration = 1;
      state->step = 0;
      wait(2);
    }
    WorldMap_StartStillPictureFar();
  }
}

/// On the World Map, loads the area graphics WorldMap_Freeze left pending and
/// lets the camera's actor move again.
void WorldMap_ReloadPendingAreaGraphics(void) {
  struct FieldWork *work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(*work));

  if (work->mode == ACTOR_SYSTEM_WORLD_MAP && work->areaGraphicsPending != 0) {
    work->areaGraphicsPending = 0;
    work->camera->paused = 0;
    WorldMap_LoadAreaGraphicsFar();
  }
}

/// Returns the actor that stands for the party: the leader's, or the first
/// placed actor while gMainRoutineWork.ownerOverride gives the party's control
/// to it (the ship, when the party sails it).
struct Actor *Field_GetPartyActor(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;

  if (party->ownerOverride == 0)
    leader = work->actors[party->leaderActorId];
  else
    leader = work->actors[ACTOR_FIRST_PLACED];
  return leader;
}

#undef HAZARD_DAMAGE_AMOUNT
#undef HEAVY_DAMAGE
#undef POISON_DAMAGE_DIVISOR
#undef VENOM_DAMAGE_DIVISOR
