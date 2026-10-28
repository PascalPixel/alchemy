// Draft, differ 37 of 171 (was 41; 6 of 160 in tla-ja before): the Kandorean
// Temple's prepare step. Japanese lacks the final flag 0x12f clear. Left: the
// native reloads gMainRoutineWork's address for the final test; this draft keeps it.
// Measured: gcse inserts the address load on the path that skips the inner
// tests and keeps it in r5 across their calls, where the native loads it into
// r2 there and again at the join. The final entrance read through a volatile
// pointer reloads it (differ 21) but reads it as ldrh and compares it shifted;
// a switch over the inner tests (46).
// Func_02005170's last argument goes through a local (41 to 37): the
// native stores the 1 and the 4 on the stack in that order (a temporary device
// if adopted).
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "edition.h"
#include "flag_call.h"

void Func_020053b8(const void *data);
void Func_02005028(void);

void Func_02002a84(void) {
  struct Actor *object = Actor_GetById(9);

  Func_020052c0(9);
  object->priorityFlags |= 2;
  Actor_SetModeById(9, 2);
  object = Actor_GetById(8);
  object->motionFlags = 0;
  if (GameFlag_Test(0x302)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1380000, 0x200000,
                                      0x1d80000);
    Func_02005178_(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Test(0x303)) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1580000, 0x200000,
                                      0x1d80000);
    Func_02005178_(0, 4, 3, 3, 19, 28);
  }
  Func_020053b8(gData_020054e0);
  object = Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->collisionFlags = 0;
  Func_02005338(0);
  if (Flag_IsSet(0x92d)) {
    {
      s32 f = 4;

      Func_02005170(41, 0, 20, 0, 1, f);
    }
    Func_02005230_(11, 0, 0);
    Func_02005230_(12, 0, 0);
    Func_02005230_(13, 0, 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    if (gMainRoutineWork.entrance == 2) {
      Actor_GetById(gMainRoutineWork.currentOwner)->y = 0x400000;
    } else if (gMainRoutineWork.entrance == 3 && Flag_IsSet(0x92d) == 0) {
      Func_02005230_(11, 0, 0);
      Func_02005230_(12, 0, 0);
      Func_02005230_(13, 0, 0);
      Func_02005028();
    }
  }
#if LANGUAGE != LANGUAGE_JAPANESE
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}
