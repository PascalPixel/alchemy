#include "types.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);

s32 Func_02001b9c(s32 x, s32 y, s32 z, s32 rotation, s32 timer);
void Func_02004c9c(s32 cue);
void FieldEvent_WaitFrames(s32 frames);

void Func_02001bf4(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Func_02001b9c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_02004c9c(151);
  FieldEvent_WaitFrames(15);
  Func_02001b9c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_02004c9c(151);
  FieldEvent_WaitFrames(15);
  Func_02001b9c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_02004c9c(151);
  FieldEvent_WaitFrames(15);
  Func_02001b9c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_02004c9c(151);
  FieldEvent_WaitFrames(15);
}
