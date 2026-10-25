// Draft, differ 16 of 194: Func_02001634's twin for the second lift
// (actors 14 and 16, gData_020062a4, flag 0x209); the same remaining
// difference.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "call.h"

extern s32 gData_020062a4;
extern u16 gData_020062aa;

void Func_02005178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005388(s32 cue);
void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000cf8(s32 id, s32 offset, s32 limit);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Math_Sine(s32 angle);

void Func_02001804(void) {
  struct ObjectRuntime *lift = Object_GetById(14);
  struct ObjectRuntime *base = Object_GetById(16);
  s32 *timer = &gData_020062a4;

  if (*timer == 300) {
    Func_02005388(162);
    Call4((void (*)())Object_SetPositionAndResetMotion, (s32)Object_GetById(14),
          0x2580000, -0x140000, 0xec0000);
    Call4((void (*)())Object_SetPositionAndResetMotion, (s32)Object_GetById(16),
          0x2580000, -0x140000, 0xe80000);
    ObjectMotion_SetActionVariant(14, 3);
    ObjectMotion_SetActionVariant(16, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Object_SetModeById(14, 0);
    Object_SetModeById(16, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += 0x40000;
    Func_02000cd8(16, 0x4ccc, 0x4ccc);
    if (lift->y > -0x50000)
      ObjectMotion_SetActionVariant(14, 2);
    if (lift->y >= 0) {
      Call6((void (*)())Func_02005178, 0, 0, 1, 1, 37, 14);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gData_020062aa;

    lift->y += Math_Sine(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      Call6((void (*)())Func_02005178, 37, 12, 1, 1, 37, 14);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Test(0x209)) {
        Call6((void (*)())Func_02005178, 22, 0, 1, 1, 37, 14);
        Scheduler_RemoveCallbackFar(Func_02001804);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += -0x40000;
    Func_02000cf8(16, 0x4ccc, 0x4ccc);
    if (lift->y < -0x50000)
      ObjectMotion_SetActionVariant(14, 3);
    if (lift->y <= -0x140000) {
      Func_02005388(245);
      Func_02005230(14, 0, 0);
      Func_02005230(16, 0, 0);
      *timer = 0;
    }
  }
  gData_020062a4++;
}
