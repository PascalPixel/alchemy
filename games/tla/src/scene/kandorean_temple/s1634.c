#include "local.h"
#include "call.h"

extern s32 gData_020062a0;
extern u16 gData_020062a8;

void Func_02005178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005388(s32 cue);
void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000cf8(s32 id, s32 offset, s32 limit);
void Task_RemoveFar(void (*func)(void));
s32 Math_Sine(s32 angle);

void Func_02001634(void) {
  struct Actor *lift = Actor_GetById(13);
  struct Actor *base = Actor_GetById(15);
  s32 *timer = &gData_020062a0;

  if (*timer == 300) {
    Func_02005388(162);
    Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(13),
          PIXELS(440), PIXELS(-20), PIXELS(204));
    Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(15),
          PIXELS(440), PIXELS(-20), PIXELS(200));
    Actor_SetSpritePriority(13, 3);
    Actor_SetSpritePriority(15, 3);
    base->priorityFlags |= 2;
    base->scaleY = 0x400;
    Actor_SetModeById(13, 0);
    Actor_SetModeById(15, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y += PIXELS(4);
    Func_02000cd8(15, 0x4ccc, 0x4ccc);
    // y read as long, another alias set than the reads around it: GCSE then
    // keeps the reload after this call; as lift->y, 2 instructions move.
    if (*(long *)&lift->y > -0x50000)
      Actor_SetSpritePriority(13, 2);
    if (lift->y >= 0) {
      Call6((void (*)())Func_02005178, 0, 0, 1, 1, 27, 12);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gData_020062a8;

    lift->y += Math_Sine(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      Call6((void (*)())Func_02005178, 27, 10, 1, 1, 27, 12);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Test(0x208)) {
        Call6((void (*)())Func_02005178, 21, 0, 1, 1, 27, 12);
        Task_RemoveFar(Func_02001634);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y += PIXELS(-4);
    Func_02000cf8(15, 0x4ccc, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y < -0x50000)
      Actor_SetSpritePriority(13, 3);
    if (lift->y <= -0x140000) {
      /* FAKEMATCH(temporary): the timer pointer's own address taken, which
       * orders the stack arguments of the 2280 call as the native does; as
       * *timer = 0, 6 of 193 instructions differ in tla-en. */
      s32 **timerRef = &timer;

      Func_02005388(245);
      Func_02005230(13, 0, 0);
      Func_02005230(15, 0, 0);
      **timerRef = 0;
    }
  }
  gData_020062a0++;
}
