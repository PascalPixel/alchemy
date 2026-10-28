#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "flag_call.h"
#include "call.h"
#include "macros.h"
#include "task.h"

struct JupiterLighthouseRoom5EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

extern u8 gData_02002fb8[];

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02002900(s32 actor, s32 x, s32 z, s32 angle);
void Func_020029c8(void *data);
void Func_02002860(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 Task_Add(void (*callback)(void), s32 priority);
void Func_020001a4(void);

/// Scene preparation: the event value 0x204, actor 22 placed while flag 0xa22
/// is set (cleared once 0xa23 is), actors 23 and 24 placed, and with flag
/// 0xa00 two more and the particle callback.
s32 Func_02000ae8(void) {
  struct Actor *object;
  s32 angle;

  ((struct JupiterLighthouseRoom5EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  if (Flag_IsSet(0xa23))
    Flag_Clear(0xa22);
  if (Flag_IsSet(0xa22)) {
    angle = 0x4000;
    Call4((void (*)())Func_02002900, 22, 0x400000, 0xe10000, angle);
  }
  Func_020029c8(gData_02002fb8);
  Actor_SetActionCallback(Actor_GetById(64), 9);
  object = Actor_GetById(23);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x200000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_GetById(23), 0);
  object = Actor_GetById(24);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_POSITION(object, 0x2a80000, 0x400000, 0x2440000);
  Actor_SetSingleChildFlags(Actor_GetById(24), 0);
  if (Flag_IsSet(0xa00)) {
    Call6((void (*)())Func_02002860, 58, 99, 3, 3, 41, 99);
    Call6((void (*)())Func_02002860, 123, 32, 1, 5, 106, 32);
    Task_Add(Func_020001a4, TASK_PASS_VBLANK);
  }
  return 0;
}
