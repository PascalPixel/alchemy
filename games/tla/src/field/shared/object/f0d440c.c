#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "map.h"
#include "object_runtime.h"

void WaitFrames(s32);

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
  struct ObjectRuntime *object;

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
  Object_ResetMotion(object);
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
    WaitFrames(1);
    if (runtimeBlock->controlMode != 3) {
      Map_CenterOnFollowedPointFar();
    }
  } else {
    Object_SetPosition(object, positionX, positionY, positionZ);
  }
}
