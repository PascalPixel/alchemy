// Near miss: Func_02000438 differs 6, as overlay 68c's Func_020012dc does: the
// listing schedules the hoisted 0x80000 and 0x1c0000 after the first position
// loads. The spawn helper takes its rotation as s32 here: with an s16 parameter
// 0xf000 becomes a pool load.
#include "types.h"
#include "object_runtime.h"
#include "flag_call.h"

struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_020003e8(s32 x, s32 y, s32 z, s32 rotation, s32 timer);
void Func_020032a8(s32 cue);
void Battle_WaitMode0(s32 frames);

void Func_02000438(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  Func_020003e8(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_020032a8(151);
  Battle_WaitMode0(15);
  Func_020003e8(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_020032a8(151);
  Battle_WaitMode0(15);
  Func_020003e8(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Func_020032a8(151);
  Battle_WaitMode0(15);
  Func_020003e8(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Func_020032a8(151);
  Battle_WaitMode0(15);
}
