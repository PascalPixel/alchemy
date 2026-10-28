#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_02002124(s32 cue);
void Func_02001fa4(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02001fac(struct Actor *object);
void Func_02001f9c(struct Actor *object);

void Func_020001d8(void) {
  struct Actor *object = Actor_GetById(13);
  struct Actor *first;
  struct Actor *second;

  Actor_SetModeById(13, 5);
  FieldEvent_WaitFrames(28);
  Func_02002124(112);
  first =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  first->speedLimit = 0x20000;
  first->acceleration = 0x20000;
  first->motionFlags = 0;
  Func_02001fa4(first, first->x + 0x60000, first->y + 0x80000, first->z);
  second =
      Actor_Spawn(674, object->x + 0xa0000, object->y + 0x100000, object->z);
  second->speedLimit = 0x20000;
  second->acceleration = 0x20000;
  second->motionFlags = 0;
  Func_02001fa4(second, first->x + 0x80000, first->y - 0x40000, first->z);
  Func_02001fac(first);
  Func_02001f9c(first);
  Func_02001fac(second);
  Func_02001f9c(second);
  FieldEvent_WaitFrames(30);
}
