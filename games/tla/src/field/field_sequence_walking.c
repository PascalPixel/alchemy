/// @file
/// Field requests and each frame of the leader's walking: exits, encounters,
/// damage.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/game.h"
#include "system/ram_buffer.h"
#include "macros.h"

/// Unused: returns 0.
UNUSED s32 FieldSequence_ReturnZero(void) { return 0; }

// The fields of struct Unit that Field_UpdateWalking reads (see its tag).
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
/// leader (a World Map cell picture on the World Map), or 0 without a leader.
s32 Field_GetCollisionAheadOfLeader(void) {
  struct FieldWork *block = Ram_HeapSlots->fieldWork;
  struct Actor *object = WorldMap_Unfreeze();
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  FIXED position[3];
  u8 *cell;

  if (object == NULL)
    return 0;
  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed;
  Vector_AddPolarOffset(toFIXED(16), object->facing, position);
  if (block->mode == 3) {
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
    cell += (x / toFIXED(16) + z / toFIXED(16) * MAPGRID_SIZE << 2);
  }
  return cell[2];
}

/// The collision code of the cell under the leader (on the world map, of its
/// block), or 0 with no leader. Teleport's triggers match it.
s32 Field_GetCollisionUnderLeader(void) {
  struct FieldWork *block = Ram_HeapSlots->fieldWork;
  struct Actor *object = WorldMap_Unfreeze();
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Vector3 position;
  struct MapCell *cell;

  if (object == NULL)
    return 0;
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  if (block->mode == 3) {
    // The block table at its fixed address: as the linked gCellPictures it
    // loads
    cell = (struct MapCell *)Ram_MapBlocks +
           (((position.x / toFIXED(32)) & 31) +
            ((position.z / toFIXED(32)) & 31) * 32);
  } else {
    cell = map->layers[0].cells;
    cell += position.x / toFIXED(16) + position.z / toFIXED(16) * 128;
  }
  return cell->collisionCode;
}

// The overlay's entry table; entry 11 returns the map's region list.

/// Takes the exit of the first of the scene's exit regions that contains the
/// position (x, y, z), in 16.16 pixels, and whose condition holds: records the
/// region's exit value for FieldSequence_Run, plays the exit sound and starts
/// leaving the scene.
void Field_CheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ) {
  s32 selectedValue;
  struct FieldWork *runtime;
  s32 z;
  s32 y;
  s32 x;
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
  region = ((struct MapRegion * (*)(void)) gOverlayArea[11])();
  if (region != 0 &&
      (runtime = Ram_HeapSlots->fieldWork, minX = region->minX, minX != -1)) {
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
      runtime->exit = (u16)selectedValue;
      Sound_PlayFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    runtime = Ram_HeapSlots->fieldWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

s32 DjinnRecovery_CountDownFar(void);
void FieldEffect_SetAttached(s32 owner, s32 mode);

/// Handles one frame of the party leader's walking onto the cell at (x, y, z)
/// of map grid layer: records step triggers and push cells, takes exits, and
/// while walking advances the encounter counter, regenerates PP, hurts the
/// party with poison and hazardous ground, and wears Avoid off.
void Field_UpdateWalking(u32 layer, s32 x, s32 y, s32 z) {
  struct FieldWork *work = gFieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 leaderId = Party_GetLeaderActor();
  struct Actor *leader = WorldMap_Unfreeze();
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
    tile = &tile[(x / toFIXED(16)) + ((z / toFIXED(16)) << 7)];
    event = tile->collisionCode;
    attributes = tile->attributeB;
  }
  work->previousCell = work->currentCell;
  work->currentCell = tile;
  if (attributes != 0)
    Field_CheckExitRegions(x, y, z);
  if (event - 1 <= 229)
    work->stepTrigger = event;
  if (attributes & 0xe2)
    work->pushCell = attributes;

  if ((gMainRoutineWork.movementMode <= 1 ||
       gMainRoutineWork.movementMode == LEADER_MOVEMENT_SHIP_ON_MAP ||
       gMainRoutineWork.movementMode == LEADER_MOVEMENT_SHIP) &&
      leader != NULL && leader->targetX != TARGET_UNSET) {
    zone = 0;
    speed = leader->speedLimit;
    if (GameFlag_Get(FLAG_LURE_CAP))
      speed <<= 1;
    if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
      work->encounter = Encounter_RollWorldMap((s32)&leader->x.fixed, speed);
    } else {
      if (attributes & 4)
        zone = 1;
      work->encounter = Encounter_RollTerrain(zone, speed);
    }

    step = Iwram_MulFX(work->walkingDistanceScale, leader->speedLimit);
    if (!zone)
      step /= 2;
    distance = work->walkingDistance + step;
    work->walkingDistance = distance;
    if (distance > 0xffff) {
      amount = distance / FIXED_ONE;
      work->walkingDistance = distance & 0xffff;
      Party_AddPp(amount, 0);
      if (DjinnRecovery_CountDownFar())
        Sound_PlayFar(SE_DJINNI_SET);
      Party_ClearUnusableShortcuts();
      partyHurt = Party_ApplyPoisonDamage();
    }

    if (gMainRoutineWork.hazardMode == 0 && (attributes & 8)) {
      if (work->previousCell->attributeB & 8)
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
          work->psynergy = 0x2096;
      }
    }
  }

  if (gMainRoutineWork.hazardMode == 1) {
    gMainRoutineWork.hazardSteps++;
    if ((s16)gMainRoutineWork.hazardSteps == gMainRoutineWork.hazardLimit / 2)
      FieldEffect_SetAttached(leaderId, 0x101);
    if ((s16)gMainRoutineWork.hazardSteps == gMainRoutineWork.hazardLimit)
      FieldEffect_SetAttached(leaderId, 0x100);
  }
  if ((s16)gMainRoutineWork.hazardSteps >= gMainRoutineWork.hazardLimit) {
    amount = gMainRoutineWork.hazardDamage;
    gMainRoutineWork.hazardSteps = 0;
    Party_AddHp(-(amount & 255), amount & 0x100);
    partyHurt++;
  }

  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    FieldEffect_SetAttached(leaderId, 0x102);
    for (i = 0; i < partyCount; i++) {
      unit = Unit_Get(gMainRoutineWork.party[i]);
      if (unit->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gMainRoutineWork.party[i];
        work->raisedTrigger = -1;
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
/// HP when percent is set.
void Party_AddHp(s32 amount, s32 scaled) {
  s32 count;
  s32 i;
  s32 value;
  struct Unit *unit;
  if (amount < 0) {
    PaletteFade_SetSource(RGB(31, 15, 0), 0);
    PaletteFade_StartInterpolation(4);
    if (amount < -10)
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
/// least 1.
s32 Party_ApplyPoisonDamage(void) {
  s32 result = 0;
  s32 count = Party_CountMembersFar();

  if (result < count) {
    s32 offset = 134;
    u8 *entry;
    s32 remaining;

    offset <<= 2;
    entry = (u8 *)&gMainRoutineWork + offset;
    remaining = count;

    do {
      struct Unit *object = Unit_Get(*entry);
      s32 amount;

      switch (object->poison) {
      case 1:
        amount = -((object->hpMax + 10) / 20);
        if (amount == 0)
          amount = -1;
        if (result <= 0)
          result = 1;
        break;
      case 2:
        amount = -((object->hpMax + 5) / 10);
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
    struct ScreenTransitionState *transition;
    s32 zero;

    work->areaGraphicsPending = 1;

    work->camera->paused = 1;
    zero = 0;
    transition =
        HeapSlot_AllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*transition));
    if (transition != NULL && transition->step != 0) {
      transition->start = 80;
      transition->end = zero;
      transition->duration = 1;
      transition->step = zero;
      wait(2);
    }
    WorldMap_StartStillPictureFar();
  }
}

/// On the world map (display mode 3), loads the area graphics a change left
/// pending.
void WorldMap_ReloadPendingAreaGraphics(void) {
  struct FieldWork *block = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, 0xd50);

  if (block->mode == ACTOR_SYSTEM_WORLD_MAP &&
      block->areaGraphicsPending != 0) {
    block->areaGraphicsPending = 0;
    block->camera->paused = 0;
    WorldMap_LoadAreaGraphicsFar();
  }
}

/// Ends what WorldMap_Freeze started: on the World Map, reloads
/// the area's graphics and sets the camera's movement state back to 0.
void *WorldMap_Unfreeze(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  void **event;

  event = Ram_HeapSlots->fieldWork;
  if (party->ownerOverride == 0) {
    s32 offset = party->leaderActorId * 4;

    offset += 20;
    event = *(void **)((u8 *)event + offset);
  } else {
    event = event[13];
  }
  return event;
}
