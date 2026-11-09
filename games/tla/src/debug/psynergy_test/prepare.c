#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "fixed_math.h"
#include "event_inline.h"

void Func_02000e0c();
void Func_02000f24();

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200091c(void) {
  Func_02000e0c();
  return;
}

void Func_02000924(void) {
  Func_02000f24();
  return;
}

void Func_0200092c(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    MapGrid_CopyCells(a, b, c, d, e, f);
}

void Func_02000950(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_Get(id);
  if (mode == 1) {
    Actor_SetPosition(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      System_Wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

void Func_020009c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct Actor *object0;

  object0 = Actor_Get(arg1);
  if (arg0 != 2)
    goto L_02008a06;
  Actor_SetPosition(arg1, (arg2 << 16), (arg3 << 16));
  object0->y = 0x200000;
  object0->motionFlags = 3;
  object0->gravity = 0x170a;
  System_Wait(50);
L_02008a06:
  return;
}
