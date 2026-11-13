/// @file
/// Field requests and each frame of the leader's walking: exits, encounters,
/// damage.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/game.h"
#include "system/ram_buffer.h"

/// Unused: returns 0.
UNUSED s32 FieldSequence_ReturnZero(void) { return 0; }

void Field_ClearRequests(void) {
  s16 value;
  struct BattleEffectCounterState *state;

  value = 0;
  state = Ram_HeapSlots->eventWork;
  state->counters[0] = value;
  state->counters[1] = 0;
  state->counters[2] = 0;
  state->counters[3] = 0;
  state->counters[4] = 0;
  state->counters[5] = 0;
  state->counters[6] = 0;
  state->counters[7] = 0;
  state->counters[8] = 0;
  state->counters[9] = 0;
  state->counters[10] = 0;
  state->counters[11] = 0;
}

s32 Field_CountRequests(void) {
  struct BattleEffectCounterState *state = Ram_HeapSlots->eventWork;

  return state->counters[0] + state->counters[1] + state->counters[2] +
         state->counters[3] + state->counters[4] + state->counters[5] +
         state->counters[6] + state->counters[7] + state->counters[8] +
         state->counters[9] + state->counters[10] + state->counters[11];
}

/// Sets event work byte 0xd39 when flag 0x8df is set and a party member
/// knows Hover (ABILITY_HOVER); clears it otherwise. The world map's pad
/// movement reads it.
void Field_UpdateHoverReady(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (GameFlag_Get(FLAG_SHIP_FLIES) && Party_FindOwnerWithActionFar(153) != -1)
    work[0xd39] = 1;
  else
    work[0xd39] = 0;
}

s32 Field_GetCollisionAheadOfLeader(void) {
  struct DisplayBlock *block = Ram_HeapSlots->eventWork;
  struct Actor *object = WorldMap_Unfreeze();
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 position[3];
  u8 *cell;

  if (object == NULL)
    return 0;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffset(0x100000, object->facing, position);
  if (block->mode == 3) {
    s32 x, z;

    x = position[0];
    z = position[2];
    cell = Ram_MapBlocks +
           ((((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)) << 2);
  } else {
    s32 x, z;

    x = position[0];
    cell = (u8 *)map->layers[0].cells;
    z = position[2];
    cell += (((x / 0x100000) + ((z / 0x100000) << 7)) << 2);
  }
  return cell[2];
}

/// The collision code of the cell under the leader (on the world map, of its
/// block), or 0 with no leader. Teleport's triggers match it.
s32 Field_GetCollisionUnderLeader(void) {
  struct DisplayBlock *block = Ram_HeapSlots->eventWork;
  struct Actor *object = WorldMap_Unfreeze();
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Vector3 position;
  struct MapCell *cell;

  if (object == NULL)
    return 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (block->mode == 3) {
    // The block table at its fixed address: as the linked gMapBlocks it loads
    // after the cell index is summed, into r2 (4 of 69 instructions differ).
    cell =
        (struct MapCell *)Ram_MapBlocks +
        (((position.x / 0x200000) & 31) + ((position.z / 0x200000) & 31) * 32);
  } else {
    cell = map->layers[0].cells;
    cell += position.x / 0x100000 + position.z / 0x100000 * 128;
  }
  return cell->collisionCode;
}

#define REGION_FIXED(value) ((value) << 16)

// The overlay's entry table; entry 11 returns the map's region list.

void Field_CheckExitRegions(s32 positionX, s32 positionY, s32 positionZ) {
  s32 selectedValue;
  struct EventWork *runtime;
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
      (runtime = Ram_HeapSlots->eventWork, minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    selectedValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 && y >= REGION_FIXED(minY) &&
        y < REGION_FIXED(maxY) && x >= REGION_FIXED(minX) &&
        x < REGION_FIXED(maxX) && z >= REGION_FIXED(minZ) &&
        z < REGION_FIXED(maxZ)) {
      runtime->countdown = (u16)selectedValue;
      Sound_PlayFar(SE_MAP_EXIT);
      Leader_StopMoving();
      return;
    }
    runtime = Ram_HeapSlots->eventWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

#define gPartyStep (*(struct PartyStepView *)&gMainRoutineWork)

s32 DjinnRecovery_CountDownFar(void);
void FieldEffect_SetAttached(s32 owner, s32 mode);

void Field_UpdateWalking(u32 layer, s32 x, s32 y, s32 z) {
  struct StepWork *work = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 leaderId = Party_GetLeaderActor();
  struct Actor *leader = WorldMap_Unfreeze();
  s16 previousHp[8];
  struct MapCell *tile;
  struct UnitStepView *unit;
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
    previousHp[i] =
        ((struct UnitStepView *)Unit_Get(gPartyStep.activeOwners[i]))->hp;

  if (*((s8 *)work + 0x18a) == 3) {
    tile = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                              (((z / 0x200000) & 31) << 5)];
    event = tile->collisionCode;
    attributes = 0;
  } else {
    if (layer <= 2)
      tile = map->layers[layer].cells;
    else
      tile = (struct MapCell *)Ram_MapCellBuffer;
    tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    event = tile->collisionCode;
    attributes = tile->attributeB;
  }
  work->previousCell = work->currentCell;
  work->currentCell = tile;
  if (attributes != 0)
    Field_CheckExitRegions(x, y, z);
  if (event - 1 <= 229)
    work->exit = event;
  if (attributes & 0xe2)
    work->pushCell = attributes;

  if ((gPartyStep.renderMode <= 1 || gPartyStep.renderMode == 6 ||
       gPartyStep.renderMode == 7) &&
      leader != NULL && leader->targetX != (s32)0x80000000) {
    zone = 0;
    speed = leader->speedLimit;
    if (GameFlag_Get(0x167))
      speed <<= 1;
    if (*((s8 *)work + 0x18a) == 3) {
      work->encounterEnemyGroup =
          Encounter_RollWorldMap((s32)&leader->x, speed);
    } else {
      if (attributes & 4)
        zone = 1;
      work->encounterEnemyGroup = Encounter_RollTerrain(zone, speed);
    }

    step = Iwram_MulQ16(work->walkingDistanceScale, leader->speedLimit);
    if (!zone)
      step /= 2;
    distance = work->walkingDistance + step;
    work->walkingDistance = distance;
    if (distance > 0xffff) {
      amount = distance / Q16_ONE;
      work->walkingDistance = distance & 0xffff;
      Party_AddPp(amount, 0);
      if (DjinnRecovery_CountDownFar())
        Sound_PlayFar(SE_139);
      Party_ClearUnusableShortcuts();
      partyHurt = Party_ApplyPoisonDamage();
    }

    if (gPartyStep.hazardMode == 0 && (attributes & 8)) {
      if (work->previousCell->attributeB & 8)
        gPartyStep.hazardSteps += leader->speedLimit / Q16_ONE;
      else
        gPartyStep.hazardSteps = gPartyStep.hazardLimit / 2;
    }

    if (gPartyStep.avoidDistance != 0 && gPartyStep.sceneVariant != 2) {
      gPartyStep.avoidDistance -= leader->speedLimit;
      if (gPartyStep.avoidDistance <= 0) {
        gPartyStep.avoidDistance = 1;
        if (work->psynergy == 0)
          work->psynergy = 0x2096;
      }
    }
  }

  if (gPartyStep.hazardMode == 1) {
    gPartyStep.hazardSteps++;
    if ((s16)gPartyStep.hazardSteps == gPartyStep.hazardLimit / 2)
      FieldEffect_SetAttached(leaderId, 0x101);
    if ((s16)gPartyStep.hazardSteps == gPartyStep.hazardLimit)
      FieldEffect_SetAttached(leaderId, 0x100);
  }
  if ((s16)gPartyStep.hazardSteps >= gPartyStep.hazardLimit) {
    amount = gPartyStep.hazardDamage;
    gPartyStep.hazardSteps = 0;
    Party_AddHp(-(amount & 255), amount & 0x100);
    partyHurt++;
  }

  if (partyHurt) {
    work->fallenCount = 0;
    work->survivorCount = 0;
    FieldEffect_SetAttached(leaderId, 0x102);
    for (i = 0; i < partyCount; i++) {
      unit = Unit_Get(gPartyStep.activeOwners[i]);
      if (unit->hp > 0) {
        work->survivorCount++;
      } else if (previousHp[i] != 0) {
        work->fallenOwners[work->fallenCount++] = gPartyStep.activeOwners[i];
        work->raisedEvent = -1;
        unit->poison = 0;
      }
    }
  }
}

void Party_AddPp(s32 amount, s32 unused) {
  s32 targetCount = Party_CountMembersFar();
  s32 i;

  for (i = 0; i < targetCount; i++)
    Unit_AddPpFar(gMainRoutineWork.activeOwners[i], amount);
}

void Party_AddHp(s32 amount, s32 scaled) {
  s32 count;
  s32 i;
  s32 value;
  struct BattleUnit *unit;
  if (amount < 0) {
    BattleEffect_ApplyColorToSourceBuffer(0x1ff, 0);
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
    unit = Unit_Get(gMainRoutineWork.activeOwners[i]);
    if (!scaled) {
      value = amount;
    } else {
      value = unit->maxHp * amount / 100;
      if (value == 0) {
        value = amount;
        if (value < 0)
          value = -value;
      }
    }
    Unit_AddHpFar(gMainRoutineWork.activeOwners[i], value);
  }
}

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
      struct BattleUnit *object = Unit_Get(*entry);
      s32 amount;

      switch (object->poison) {
      case 1:
        amount = -((object->maxHp + 10) / 20);
        if (amount == 0)
          amount = -1;
        if (result <= 0)
          result = 1;
        break;
      case 2:
        amount = -((object->maxHp + 5) / 10);
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
    BattleEffect_ApplyColorToSourceBuffer(0x1ff, 0);
    PaletteFade_StartInterpolation(4);
    Sound_PlayFar(SE_IMPACT);
  }

  return result;
}

/// In control mode 3, marks the event work busy, shows its object and, when a
/// fade is pending, starts it at level 80 and waits 2 frames; then calls
/// MapAnimation_StartFar.
void WorldMap_Freeze(void) {
  struct FxWork *work = HeapSlot_AllocEwram(108, 0xd50);

  if (*(u16 *)&work->state == 3) {
    struct FadeWork *fade;
    s32 zero;

    work->busy = 1;

    work->sprite->visible = 1;
    zero = 0;
    fade = HeapSlot_AllocEwram(124, 0x540);
    if (fade != NULL && fade->pending != 0) {
      fade->level = 80;
      fade->step = zero;
      fade->active = 1;
      fade->pending = zero;
      wait(2);
    }
    MapAnimation_StartFar();
  }
}

/// On the world map (display mode 3), loads the area graphics a change left
/// pending.
void WorldMap_ReloadPendingAreaGraphics(void) {
  struct DisplayBlock *block = HeapSlot_AllocEwram(108, 0xd50);

  if (block->mode == 3 && block->pending != 0) {
    block->pending = 0;
    block->object[91] = 0;
    WorldMap_LoadAreaGraphicsFar();
  }
}

void *WorldMap_Unfreeze(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  void **event;

  event = Ram_HeapSlots->eventWork;
  if (party->ownerOverride == 0) {
    s32 offset = party->currentOwner * 4;

    offset += 20;
    event = *(void **)((u8 *)event + offset);
  } else {
    event = event[13];
  }
  return event;
}
