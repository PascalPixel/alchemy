/// @file
/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002f80 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"
#include "event_inline.h"

void Func_02002f80(void);

void Func_02003304(struct Actor *self) {
  struct Actor *actor = Actor_Get(((u8 *)self)[0x62]);
  s32 angle = (s16)self->unk66;
  struct FieldSprite *sprite = actor->animation;
  // volatile: the call reads position[0] before [1] and [2] (as a plain
  // array, 2 of 86 differ in tla-en).
  volatile s32 position[3];

  position[0] = self->x;
  position[1] = self->y - PIXELS(12);
  position[2] = self->z;
  FieldVector_AddPolarOffset(PIXELS(32), angle, (struct Vector3 *)position);
  actor->angle = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1], position[2]);
  actor->update = Func_02002f80;
  actor->actionFlags = 0;
  actor->speedLimit = 0x80000;
  actor->acceleration = 0x2000;
  sprite->rotation = angle - 0x4000;
  actor->scaleX = 0;
  // A once-only block: its end holds the order around it (as the plain
  // statement, 4 of 86 differ in tla-en).
  do {
    actor->scaleY = 0;
  } while (0);
  actor->action = 0;
  actor->watchedObject = self->watchedObject;
  Event_PlaySound(104);
}
