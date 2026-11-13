/// @file
/// The view bounds, and effect objects linked to another object.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/field.h"

s32 __divsi3(s32, s32);

/// Sets the map coordinates the camera keeps the screen inside.
void MapGrid_SetCameraBounds(s32 minX, s32 minZ, s32 maxX, s32 maxZ) {
  struct MapWork *map = Ram_HeapSlots->mapWork;

  map->minX = minX;
  map->minZ = minZ;
  map->maxX = maxX;
  map->maxZ = maxZ;
}

void WorldMap_StepZoom(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *duration;
  s32 *from;
  s16 *step;
  s32 offset;
  s32 delta;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))
          ->viewCenter->movementState != 0)
    return;
  duration = &map->zoomFrames;
  if (*duration == 0)
    return;
  from = &map->zoomFrom;
  delta = map->zoomTo - *from;
  step = &map->zoomStep;
  (*step)++;
  offset = *from + __divsi3(delta * *step, *duration);
  map->zoom = Iwram_MulQ16(map->farPlane, offset);
  gScanlinePitch = *(u16 *)((u8 *)map + 0x11c) + 1;
  if (*step == *duration) {
    *duration = 0;
    Task_Remove(WorldMap_StepZoom);
  }
}

void WorldMap_StartZoom(s32 zoom, s32 frames) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 result;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->controlMode != 3)
    return;
  {
    s32 (*ratio)(s32, s32) = Iwram_RatioMulQ14;
    result = ratio(zoom, 0x10000);
  }
  map->zoomFrom = map->zoomTo;
  map->zoomTo = result;
  map->zoomFrames = frames;
  map->zoomStep = 0;
  Task_Add(WorldMap_StepZoom, TASK_PASS_VBLANK + 20);
}

void Event_WaitForZoomTransition(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 frames;

  if (((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->controlMode == 3) {
    frames = 0;
    if (map->zoomFrames != 0) {
      do {
        wait(1);
        frames++;
      } while (frames <= 0x12b && map->zoomFrames != 0);
    }
  }
}

s32 FieldEffect_CopyWatchedActorPosition(struct Actor *object) {
  struct Actor *link;

  link = object->watchedObject;
  if (link != NULL) {
    object->motionFlags = 0;
    object->x = link->x;
    object->y = link->y +
                ((s8)Resource_GetMetadataRecordFarFar(
                     *(s16 *)Scene_FindActorPlacement((s16)object->unk66))
                     ->boxX
                 << 16) +
                0x80000;
    object->terrainHeight = link->terrainHeight;
    object->z = link->z;
  }
  return 0;
}

struct BattleEffectWatchedObject *Actor_CreateFar(s32 kind, s32 x, s32 y,
                                                  s32 z);
extern const u8 gFieldEffect_WatchedActorScript[];

/// The Broken Seal's BattleEffect_SpawnLinked with this game's object kind
/// (488): spawns an effect object at object resourceId that follows it,
/// waits state, and returns the new object.
struct BattleEffectWatchedObject *Actor_ShowEmoteById(s32 resourceId, s32 flags,
                                                      s32 state) {
  struct BattleEffectResource *resource;
  struct BattleEffectWatchedObject *object;

  if ((flags & 0xff) == 6) {
    Sound_PlayFar(110);
  }

  resource = ObjectTable_Get(resourceId);
  if (resource != 0) {
    object = Actor_CreateFar(488, resource->x, resource->y, resource->z);

    if (object != 0) {
      Actor_StartScriptFar((struct Actor *)object,
                           (u32)gFieldEffect_WatchedActorScript);
      Actor_SelectAnimationFar(object, flags & 15);
      object->value55 = 0;
      object->counter = 0;
      object->resourceId = resourceId;
      object->callback = FieldEffect_CopyWatchedActorPosition;
      object->resource = resource;
      object->visual->value26 = 0;

      if ((flags & 0x100) != 0) {
        s32 mask = 13;
        u8 visualFlags = object->visual->flags;

        mask = -mask;
        mask &= visualFlags;
        mask |= 4;
        object->visual->flags = mask;
      } else {
        s32 copiedFlags = 12;
        u8 sourceFlags = resource->visual->flags;
        u8 flags;
        s32 clearMask = 13;

        copiedFlags &= sourceFlags;
        flags = object->visual->flags;
        clearMask = -clearMask;
        clearMask &= flags;
        clearMask |= copiedFlags;
        object->visual->flags = clearMask;
      }
    }
    Event_Wait(state);
  }
  return object;
}

s32 Actor_IsOutsideAnimation108(struct Actor *object) {
  struct ObjectAnimationState *state;
  s32 value;

  if (object == 0 || object->animationKind != 1 ||
      (state = object->sprite) == 0 || state->value28 == 0)
    return 1;
  value = *state->value28;
  value ^= 0x108;
  if (value != 0)
    return 1;
  return 0;
}
