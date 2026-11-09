#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "macros.h"
#include "task.h"
#include "scene_tables.h"
#include "event_inline.h"

struct JupiterLighthouseRoom5EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

extern u8 gData_02002fb8[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_020029c8(void *data);
void Func_020001a4(void);

/// Scene preparation: the event value 0x204, actor 22 placed while flag 0xa22
/// is set (cleared once 0xa23 is), actors 23 and 24 placed, and with flag
/// 0xa00 two more and the particle callback.
s32 Func_02000ae8(void) {
  struct Actor *object;
  s32 angle;

  ((struct JupiterLighthouseRoom5EventWork1ac *)Ram_HeapSlots->eventWork)
      ->value = 0x204;
  if (Event_GetFlag(0xa23))
    Event_ClearFlag(0xa22);
  if (Event_GetFlag(0xa22)) {
    angle = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(64), PIXELS(225), angle);
  }
  Func_020029c8(gData_02002fb8);
  Actor_SetActionCallback(Actor_Get(64), 9);
  object = Actor_Get(23);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x200000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_Get(23), 0);
  object = Actor_Get(24);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x400000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_Get(24), 0);
  if (Event_GetFlag(0xa00)) {
    MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
    MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
    Task_AddFrame(Func_020001a4, TASK_PASS_VBLANK);
  }
  return 0;
}
