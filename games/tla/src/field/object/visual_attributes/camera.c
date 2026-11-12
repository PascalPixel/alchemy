/// @file
/// Routines whose twins are in The Broken Seal's field/event_commands/camera.c.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

void Camera_MoveWithinMapBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                                s32 useSetter) {
  s32 shouldUseSetter;
  struct EventWork *runtimeBlock;
  s32 minimumX;
  s32 minimumZ;
  s32 maximumX;
  s32 objectZOffset;
  s32 maximumZ;
  s32 positionX;
  s32 positionZ;
  s32 positionY;
  struct MapWork *map;
  struct Actor *object;

  positionX = requestedX;
  positionY = requestedY;
  shouldUseSetter = useSetter;
  positionZ = requestedZ;
  runtimeBlock = HeapSlot_AllocEwram(108, 0xd50);
  object = runtimeBlock->viewCenter;
  map = Ram_HeapSlots->mapWork;
  minimumX = map->minX + 0x780000;
  objectZOffset = object->y;
  minimumZ = map->minZ + objectZOffset + 0x600000;
  maximumX = map->maxX + 0xFF880000;
  maximumZ = map->maxZ + objectZOffset + 0xFFC00000;
  map->origin = &object->x;
  Actor_ResetMotion(object);
  if (positionX == -1) {
    positionX = object->x;
  }
  if (positionY == -1) {
    positionY = object->y;
  }
  if (positionZ == -1) {
    positionZ = object->z;
  }
  if (positionX < minimumX) {
    positionX = minimumX;
  }
  if (positionZ < minimumZ) {
    positionZ = minimumZ;
  }
  if (positionX > maximumX) {
    positionX = maximumX;
  }
  if (positionZ > maximumZ) {
    positionZ = maximumZ;
  }
  if (shouldUseSetter == 0) {
    object->x = positionX;
    object->y = positionY;
    object->z = positionZ;
    wait(1);
    if (runtimeBlock->controlMode != 3) {
      Map_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetTargetPosition(object, positionX, positionY, positionZ);
  }
}


s32 __divsi3(s32, s32);
extern u32 gData_03001144;

void Func_080d450c(s32 id, s32 use_setter) {
  struct Actor *obj = ObjectTable_Get(id);

  if (obj != NULL)
    Camera_MoveWithinMapBounds(obj->x, -1, obj->z, use_setter);
}

void Func_080d452c(s32 id, s32 use_setter) {
  struct Actor *obj = ObjectTable_Get(id);

  if (obj != NULL)
    Camera_MoveWithinMapBounds(obj->x, obj->y, obj->z, use_setter);
}

/// Waits (at most 600 frames) for the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_CommitPosition(
      ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter);
  Event_Wait(2);
}

struct Actor *Func_080d4568(void) {
  return ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter;
}

void Func_080d4580(struct Actor *target, s32 keepCurrentPosition) {
  struct Actor *object;

  object = ((struct EventWork *)HeapSlot_AllocEwram(108, 0xd50))->viewCenter;
  if (target != NULL) {
    Actor_StartCameraFar(object, NULL);
    object->watchedObject = target;
    if (keepCurrentPosition == 0) {
      SET_POSITION(object, target->x, target->y, target->z);
    }
  }
}
