#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02000dac();
void Func_02000e74();
struct ObjectRuntime *Object_GetById();
void WaitFrames();

void Func_02000e0c();
void Func_02000f24();

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200091c(void) {
  ((void (*)())Func_02000e0c)();
  return;
}

void Func_02000924(void) {
  ((void (*)())Func_02000f24)();
  return;
}

void Func_0200092c(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    Func_02000dac(a, b, c, d, e, f);
}

void Func_02000950(s32 mode, s32 id, s32 x, s32 z) {
  struct ObjectRuntime *object;
  s32 scale;

  object = Object_GetById(id);
  if (mode == 1) {
    Func_02000e74(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    /* FAKEMATCH: ⚓️ loads the scale before it builds the bound. */
    __asm__ volatile("" : "+l"(object));
    while (object->scaleX < 0x10000) {
      WaitFrames(1);
      scale = object->scaleX;
      /* FAKEMATCH: and loads it again before it builds the step. */
      __asm__ volatile("" : "+l"(scale));
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
  }
}

void Func_020009c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct ObjectRuntime *object0;

  object0 = Object_GetById(arg1);
  if (arg0 != 2)
    goto L_02008a06;
  Call3((void (*)())Func_02000e74, arg1, (arg2 << 16), (arg3 << 16));
  object0->y = 0x200000;
  object0->motionFlags = 3;
  object0->gravity = 0x170a;
  Call1((void (*)())WaitFrames, 50);
L_02008a06:
  return;
}
