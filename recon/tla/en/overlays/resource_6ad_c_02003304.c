// Draft of overlay 6ad's Func_02003304, the twin of overlay 6ac's
// Func_02002e6c: its draft (resource_6ac_c_02002e6c.c) with this overlay's
// symbols: differ 12 of 86. Left: what that draft's header records.
#include "../../../../games/tla/src/scene/mars_lighthouse_room_m/mars_lighthouse_room_m.h"

void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void Func_02002f80(void);

void Func_02003304(struct Actor *self) {
  struct Actor *object = Actor_GetById(((u8 *)self)[0x62]);
  s32 angle = (s16)self->unk66;
  struct MarsLighthouseRoomMAnimation *animation = object->animation;
  s32 position[3];

  position[0] = self->x;
  position[1] = self->y - 0xc0000;
  position[2] = self->z;
  Vector_AddPolarOffsetFar(0x200000, angle, position);
  object->angle = angle;
  object->motionFlags = 0;
  Actor_SetPositionAndResetMotion(object, position[0], position[1],
                                   position[2]);
  object->update = Func_02002f80;
  object->actionFlags = 0;
  object->speedLimit = 0x80000;
  object->acceleration = 0x2000;
  animation->angle = angle - 0x4000;
  object->scaleX = 0;
  object->scaleY = 0;
  object->action = 0;
  object->watchedObject = self->watchedObject;
  Func_020056c0(104);
}
