#include "mars_lighthouse_room_m.h"

void Func_02001e38(struct Actor *object) {
  struct Actor *target = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 vx;
  s32 vz;
  s32 rate;
  struct MarsLighthouseRoomMAnimation *animation;

  vx = object->bounce;
  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH: the ARM9 scheduling would load the scale rate before storing the
   * Z velocity. */
  __asm__("");
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->animation;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: and would address the spin before storing the Y scale. */
  __asm__("");
  animation->angle += object->action;
  if (target->y == target->terrainHeight && Func_02001de0(object, target)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}
