#include "local.h"
#include "call.h"

extern s32 gData_020062a4;
extern u16 gData_020062aa;
extern s32 gData_0200629c;

void Func_02005178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000cf8(s32 id, s32 offset, s32 limit);
s32 Func_02000d3c(void);
void Func_02000e60(void);
void Task_RemoveFar(void (*func)(void));
s32 Math_Sine(s32 angle);

// Bob the statue on a sine.
#define BOB(statue, phase)                                                     \
  do {                                                                         \
    (statue)->y += Math_Sine(*(phase)) >> 1;                                   \
    *(phase) += 0x1000;                                                        \
  } while (0)

void Func_020012c4(void) {
  s32 *timer = &gData_020062a4;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 raised;
  s32 last;
  s32 t;

  t = *timer;
  if (t < 300) {
    if (Func_02000d3c() != 0)
      return;
  } else if (t == 300) {
    Call6((void (*)())Func_02005178, 15, 15, 1, 1, 23, 15);
    Call6((void (*)())Func_02005178, 65, 15, 1, 1, 73, 15);
    Func_02000e60();
  } else {
    last = 2999;
    if (t <= last) {
      statue = Actor_GetById(10);
      raised = GameFlag_Test(0x300);
      t = *timer;
      if (raised) {
        if (t <= 316) {
          statue->y += 0x48000;
          limit = 0x1000;
          Func_02000cd8(12, 0x20000, limit);
          Func_02000cd8(17, 0x18000, limit);
          Func_02000cd8(18, 0x10000, limit);
          Func_02000cd8(19, 0x8000, limit);
          Func_02000cd8(20, 0, limit);
        } else if (t < 600) {
          if (GameFlag_Test(0x209) && *timer == 400) {
            Call6((void (*)())Func_02005178, 23, 0, 1, 1, 23, 15);
            Task_RemoveFar(Func_020012c4);
          }
          phase = &gData_020062aa;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gData_020062aa;
          if (*phase == 0) {
            gData_0200629c = 0;
            *timer = 999;
            GameFlag_ClearBit(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x6c000;
          limit = 0x1800;
          Func_02000cf8(12, 0x30000, limit);
          Func_02000cf8(17, 0x24000, limit);
          Func_02000cf8(18, 0x18000, limit);
          Func_02000cf8(19, 0xc000, limit);
          Func_02000cf8(20, 0, limit);
          if (Actor_GetById(20)->scaleY <= limit)
            *timer = last;
        }
      } else {
        if (t <= 316) {
          statue->y += 0x27999;
          limit = 0x1000;
          Func_02000cd8(12, 0x10000, limit);
          Func_02000cd8(17, 0x8000, limit);
          Func_02000cd8(18, 0, limit);
        } else if (t < 600) {
          phase = &gData_020062aa;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gData_020062aa;
          if (*phase == 0) {
            gData_0200629c = 0;
            *timer = 999;
            GameFlag_ClearBit(0x20d);
          }
          BOB(statue, phase);
        } else {
          statue->y += -0x3b666;
          limit = 0x1800;
          Func_02000cf8(12, 0x18000, limit);
          Func_02000cf8(17, 0xc000, limit);
          Func_02000cf8(18, 0, limit);
          if (Actor_GetById(18)->scaleY <= limit)
            *timer = last;
        }
      }
      if (GameFlag_Test(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        leader->y = statue->y + 0x40000;
        leader->targetY = 0x80000000;
        leader->velocityY = 0;
      }
    } else if (t == 3000) {
      Call6((void (*)())Func_02005178, 8, 0, 1, 1, 23, 15);
      Call6((void (*)())Func_02005178, 58, 0, 1, 1, 73, 15);
      Func_02005230(12, 0, 0);
      Func_02005230(17, 0, 0);
      Func_02005230(18, 0, 0);
      Func_02005230(19, 0, 0);
      Func_02005230(20, 0, 0);
      Actor_SetModeById(10, 1);
      Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)Actor_GetById(10),
            0x1780000, 0, 0xf80000);
      Actor_GetById(10)->priorityFlags |= 2;
      Actor_SetSpritePriority(10, 3);
      if (GameFlag_Test(0x209)) {
        leader = Actor_GetById(gMainRoutineWork.currentOwner);
        Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
        Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                  1);
      }
    } else {
      s32 none = Func_02000d3c();

      if (none != 0)
        return;
      *timer += (Random16Far() << 7) >> 16;
      if ((u32)*timer > ((Random16Far() * 10000) >> 16) + 10000)
        *timer = none;
    }
  }
  gData_020062a4++;
}
