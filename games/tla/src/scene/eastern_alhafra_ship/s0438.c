#include "types.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);

s32 Func_020003e8(s32 x, s32 y, s32 z, s32 rotation, s32 timer);
void Func_020032a8(s32 cue);
void FieldEvent_WaitFrames(s32 frames);

void Func_02000438(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Func_020003e8(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_020032a8(151);
  FieldEvent_WaitFrames(15);
  Func_020003e8(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_020032a8(151);
  FieldEvent_WaitFrames(15);
  Func_020003e8(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_020032a8(151);
  FieldEvent_WaitFrames(15);
  Func_020003e8(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_020032a8(151);
  FieldEvent_WaitFrames(15);
}
