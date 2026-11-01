// Draft, differ 21 of 369, skeleton gap 2 (tla-en, 2026-11-01)
// The Kandorean Temple statue (actor 9) and its steps, run by gData_020062a0:
// they rise, bob on a sine and sink again, carrying the leader while flag 0x208
// is set; Func_020012c4 is its twin for actor 10. Each arm of flag 0x301 is
// written out in full: cross-jumping merges their tails, and 999 and 2999 stay
// in registers.
// Left: one instruction, and the 20 differ lines are its branch and pool
// distances. At timer 3000 the native builds 0xd80000 twice, in r1 and in r3;
// CSE here builds it once and copies. Tried without change: locals for x and z,
// tile << 16, PIXELS(216), an unsigned constant, a cast call, Call4 and an
// inline wrapper, const parameters included. The native's pair looks like
// direct moves into the argument registers, which would also settle the same
// pair in Func_020012c4.
// Note for adoption: flag is a forced temporary, so *timer loads after the call
// and before the test; written as if (GameFlag_Test(0x301)) with a load in each
// arm the gap is 3. It needs a temporary tag or a better form.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "call.h"

extern s32 gData_020062a0;
extern u16 gData_020062a8;
extern s32 gData_02006298;

void Func_02005178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000cd8(s32 id, s32 offset, s32 limit);
void Func_02000cf8(s32 id, s32 offset, s32 limit);
s32 Func_02000d18(void);
void Func_02000d64(void);
void Task_RemoveFar(void (*func)(void));
s32 Math_Sine(s32 angle);

void Func_02000f60(void) {
  s32 *timer = &gData_020062a0;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 timerEnd;
  s32 t;
  s32 flag;

  if (*timer < 300) {
    if (Func_02000d18() != 0)
      return;
    goto count;
  }
  if (*timer == 300) {
    Call6((void (*)())Func_02005178, 15, 15, 1, 1, 13, 13);
    Call6((void (*)())Func_02005178, 65, 15, 1, 1, 63, 13);
    Func_02000d64();
    goto count;
  }
  timerEnd = 2999;
  if (*timer <= timerEnd) {
    statue = Actor_GetById(9);
    flag = GameFlag_Test(0x301);
    t = *timer;
    if (flag) {
      if (t <= 316) {
        statue->y += 0x48000;
        limit = 0x1000;
        Func_02000cd8(11, 0x20000, limit);
        Func_02000cd8(13, 0x18000, limit);
        Func_02000cd8(14, 0x10000, limit);
        Func_02000cd8(15, 0x8000, limit);
        Func_02000cd8(16, 0, limit);
      } else if (t < 600) {
        if (GameFlag_Test(0x208) && *timer == 400) {
          Call6((void (*)())Func_02005178, 22, 0, 1, 1, 13, 13);
          Task_RemoveFar(Func_02000f60);
        }
        phase = &gData_020062a8;
        statue->y += Math_Sine(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gData_020062a8;
        if (*phase == 0) {
          gData_02006298 = 0;
          *timer = 999;
          GameFlag_ClearBit(0x20c);
        }
        statue->y += Math_Sine(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x6c000;
        limit = 0x1800;
        Func_02000cf8(11, 0x30000, limit);
        Func_02000cf8(13, 0x24000, limit);
        Func_02000cf8(14, 0x18000, limit);
        Func_02000cf8(15, 0xc000, limit);
        Func_02000cf8(16, 0, limit);
        if (Actor_GetById(16)->scaleY <= limit)
          *timer = timerEnd;
      }
    } else {
      if (t <= 316) {
        statue->y += 0x27999;
        limit = 0x1000;
        Func_02000cd8(11, 0x10000, limit);
        Func_02000cd8(13, 0x8000, limit);
        Func_02000cd8(14, 0, limit);
      } else if (t < 600) {
        phase = &gData_020062a8;
        statue->y += Math_Sine(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gData_020062a8;
        if (*phase == 0) {
          gData_02006298 = 0;
          *timer = 999;
          GameFlag_ClearBit(0x20c);
        }
        statue->y += Math_Sine(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y += -0x3b666;
        limit = 0x1800;
        Func_02000cf8(11, 0x18000, limit);
        Func_02000cf8(13, 0xc000, limit);
        Func_02000cf8(14, 0, limit);
        if (Actor_GetById(14)->scaleY <= limit)
          *timer = timerEnd;
      }
    }
    if (GameFlag_Test(0x208) == 0)
      goto count;
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    leader->y = statue->y + 0x40000;
    leader->targetY = 0x80000000;
    leader->velocityY = 0;
    goto count;
  }
  if (*timer == 3000) {
    Call6((void (*)())Func_02005178, 8, 0, 1, 1, 13, 13);
    Call6((void (*)())Func_02005178, 58, 0, 1, 1, 63, 13);
    Func_02005230(11, 0, 0);
    Func_02005230(13, 0, 0);
    Func_02005230(14, 0, 0);
    Func_02005230(15, 0, 0);
    Func_02005230(16, 0, 0);
    Actor_SetModeById(9, 1);
    Actor_SetPositionAndResetMotion(Actor_GetById(9), 0xd80000, 0, 0xd80000);
    Actor_GetById(9)->priorityFlags |= 2;
    Actor_SetSpritePriority(9, 3);
    if (GameFlag_Test(0x208)) {
      leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Actor_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
      Actor_SetSingleChildFlags(
          Actor_GetById(gMainRoutineWork.currentOwner), 1);
    }
    goto count;
  }
  {
    s32 none = Func_02000d18();

    if (none != 0)
      return;
    *timer += (Random16Far() << 7) >> 16;
    if ((u32)*timer > ((Random16Far() * 10000) >> 16) + 10000)
      *timer = none;
  }
count:
  gData_020062a0++;
}
