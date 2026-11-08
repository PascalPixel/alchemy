// Matches (tla-en, 2026-11-08, -mtune=gba).
// Draft of overlay 6AC Func_02002e6c (0x0200ae6c).
#include "../../../../games/tla/src/scene/mars_lighthouse_room_m/mars_lighthouse_room_m.h"

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Func_02002ae8(void);

void Func_02002e6c(struct Actor *self) {
  struct Actor *object = Actor_GetById(((u8 *)self)[0x62]);
  s32 angle = (s16)self->unk66;
  struct MarsLighthouseRoomMAnimation *animation = object->animation;
  // volatile: the call reads position[0] before [1] and [2] (as a plain
  // array, 2 of 86 differ in tla-en).
  volatile s32 position[3];

  position[0] = self->x;
  position[1] = self->y - 0xc0000;
  position[2] = self->z;
  Vector_AddPolarOffsetFar(0x200000, angle, (s32 *)position);
  object->angle = angle;
  object->motionFlags = 0;
  Actor_SetPositionAndResetMotion(object, position[0], position[1],
                                   position[2]);
  object->update = Func_02002ae8;
  object->actionFlags = 0;
  object->speedLimit = 0x80000;
  object->acceleration = 0x2000;
  animation->angle = angle - 0x4000;
  object->scaleX = 0;
  // A once-only block: its end holds the order around it (as the plain
  // statement, 4 of 86 differ in tla-en).
  do {
    object->scaleY = 0;
  } while (0);
  object->action = 0;
  object->watchedObject = self->watchedObject;
  Func_020032b8(104);
}
