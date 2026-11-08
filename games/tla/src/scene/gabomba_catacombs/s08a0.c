#include "local.h"

void Func_020008a0(struct Actor *object, s32 dz) {
  struct GabombaCatacombsFollower *follower;
  struct GabombaCatacombsVec position;

  follower =
      ((struct GabombaCatacombsEventWork *)Ram_HeapSlots->eventWork)->follower;

  position.x = object->x;
  position.y = object->y;
  position.z = object->z + dz;
  Func_02001174(object, &position);
  position.x = object->x;
  position.y = object->y;
  position.z = object->z + 0x80000;
  if (Func_02001174(object, &position) <= 0) {
    position.x = object->x + 0x5b333;
    position.y = object->y;
    position.z = object->z + 0x5b333;
    if (Func_02001174(object, &position) <= 0) {
      position.x = object->x - 0x5b333;
      position.y = object->y;
      position.z = object->z + 0x5b333;
      if (Func_02001174(object, &position) <= 0) {
        follower->z += dz;
        object->z += dz;
      }
    }
  }
}
