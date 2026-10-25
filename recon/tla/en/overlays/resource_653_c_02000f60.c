// Draft, differ 121 of 369: the Kandorean Temple statue (actor 9) and its
// steps, run by gData_020062a0: they rise, bob on a sine and sink again,
// carrying the leader while flag 0x208 is set; Func_020012c4 is its twin for
// actor 10. Left: the native keeps 2999 in r8 for the timer reset and 999 in
// r1 for the compare and the store, and some calls share their constants.
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
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Math_Sine(s32 angle);

void Func_02000f60(void) {
  s32 *timer = &gData_020062a0;
  struct ObjectRuntime *statue;
  struct ObjectRuntime *leader;
  u16 *phase;
  s32 limit;
  s32 id;
  s32 t;

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
  if (*timer <= 2999) {
    statue = Object_GetById(9);
    if (GameFlag_Test(0x301)) {
      t = *timer;
      if (t <= 316) {
        statue->y += 0x48000;
        limit = 0x1000;
        Func_02000cd8(11, 0x20000, limit);
        Func_02000cd8(13, 0x18000, limit);
        Func_02000cd8(14, 0x10000, limit);
        Func_02000cd8(15, 0x8000, limit);
        Func_02000cd8(16, 0, limit);
        goto carry;
      }
      if (t < 600) {
        if (GameFlag_Test(0x208) && *timer == 400) {
          Call6((void (*)())Func_02005178, 22, 0, 1, 1, 13, 13);
          Scheduler_RemoveCallbackFar(Func_02000f60);
        }
        phase = &gData_020062a8;
        goto bob;
      }
      if (t > 999) {
        statue->y += -0x6c000;
        limit = 0x1800;
        Func_02000cf8(11, 0x30000, limit);
        Func_02000cf8(13, 0x24000, limit);
        Func_02000cf8(14, 0x18000, limit);
        Func_02000cf8(15, 0xc000, limit);
        Func_02000cf8(16, 0, limit);
        id = 16;
        goto shrink;
      }
    } else {
      t = *timer;
      if (t <= 316) {
        statue->y += 0x27999;
        limit = 0x1000;
        Func_02000cd8(11, 0x10000, limit);
        Func_02000cd8(13, 0x8000, limit);
        Func_02000cd8(14, 0, limit);
        goto carry;
      }
      if (t < 600) {
        phase = &gData_020062a8;
        goto bob;
      }
      if (t > 999) {
        statue->y += -0x3b666;
        limit = 0x1800;
        Func_02000cf8(11, 0x18000, limit);
        Func_02000cf8(13, 0xc000, limit);
        Func_02000cf8(14, 0, limit);
        id = 14;
        goto shrink;
      }
    }
    phase = &gData_020062a8;
    if (*phase == 0) {
      gData_02006298 = 0;
      *timer = 999;
      GameFlag_ClearBit(0x20c);
    }
  bob:
    statue->y += Math_Sine(*phase) >> 1;
    *phase += 0x1000;
    goto carry;
  shrink:
    if (Object_GetById(id)->scaleY <= limit)
      *timer = 2999;
  carry:
    if (GameFlag_Test(0x208) == 0)
      goto count;
    leader = Object_GetById(gPartyState.currentOwner);
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
    Object_SetModeById(9, 1);
    Object_SetPositionAndResetMotion(Object_GetById(9), 0xd80000, 0, 0xd80000);
    Object_GetById(9)->priorityFlags |= 2;
    ObjectMotion_SetActionVariant(9, 3);
    if (GameFlag_Test(0x208)) {
      leader = Object_GetById(gPartyState.currentOwner);
      Object_SetPositionAndResetMotion(leader, leader->x, 0, leader->z);
      ObjectDispatch_SetSingleChildFlags(
          Object_GetById(gPartyState.currentOwner), 1);
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
