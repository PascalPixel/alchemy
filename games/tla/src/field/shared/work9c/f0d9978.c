#include "types.h"
#include "input.h"
#include "actor.h"
#include "adventure.h"

extern s32 gData_080f3944;

void Func_080d8fa8(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080d9978(struct Actor *object) {
  u8 *parts = object->animation;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gData_080f3944 = 1;
  if (gData_080f3944 != 0)
    Func_080d8fa8(0, 12, 13, 0);
}

struct Actor *Actor_GetById(s32 id);

s32 Func_080d99b8(s32 firstId, s32 secondId) {
  struct Actor *owner = Actor_GetById(gAdventure.currentOwner);
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  s32 x = owner->x >> 20;
  s32 y = owner->y >> 20;
  s32 z = owner->z >> 20;
  s32 firstX = first->x >> 20;
  s32 firstY = first->y >> 20;
  s32 firstZ = first->z >> 20;
  s32 secondX = second->x >> 20;
  s32 secondY = second->y >> 20;
  s32 secondZ = second->z >> 20;

  if (y == firstY) {
    if (x == firstX) {
      if (z == firstZ - 1 || z == firstZ + 1 || z == firstZ)
        return 1;
    } else if (z == firstZ) {
      if (x == firstX - 1 || x == firstX + 1 || x == firstX)
        return 1;
    }
  }
  if (y == secondY) {
    if (x == secondX) {
      if (z == secondZ - 1 || z == secondZ + 1 || z == secondZ)
        return 1;
    } else if (z == secondZ) {
      if (x == secondX - 1 || x == secondX + 1 || x == secondX)
        return 1;
    }
  }
  return 0;
}
