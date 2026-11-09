#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02001f9c(struct Actor *object);

void Func_020001d8(void) {
  struct Actor *object = Actor_Get(13);
  struct Actor *first;
  struct Actor *second;

  Actor_SetAnimation(13, 5);
  FieldEvent_Wait(28);
  Event_PlaySound(112);
  first =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  first->speedLimit = 0x20000;
  first->acceleration = 0x20000;
  first->motionFlags = 0;
  FieldActor_SetMoveTarget(first, first->x + PIXELS(6), first->y + PIXELS(8),
                      first->z);
  second =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  second->speedLimit = 0x20000;
  second->acceleration = 0x20000;
  second->motionFlags = 0;
  FieldActor_SetMoveTarget(second, first->x + PIXELS(8), first->y - PIXELS(4),
                      first->z);
  Actor_WaitForMove(first);
  Func_02001f9c(first);
  Actor_WaitForMove(second);
  Func_02001f9c(second);
  FieldEvent_Wait(30);
}
