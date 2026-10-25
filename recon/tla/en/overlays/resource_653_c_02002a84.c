// Draft, differ 41 of 171 (6 of 160 in tla-ja): the Kandorean Temple's
// prepare step. Japanese lacks the final flag 0x12f clear. Left: the native
// reloads gPartyState's address for the final test; this draft keeps it.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "edition.h"
#include "flag_call.h"

void Func_020053b8(const void *data);
void Func_02005028(void);

void Func_02002a84(void) {
  struct ObjectRuntime *object = Object_GetById(9);

  Func_020052c0(9);
  object->priorityFlags |= 2;
  Object_SetModeById(9, 2);
  object = Object_GetById(8);
  object->motionFlags = 0;
  if (GameFlag_Test(0x302)) {
    Object_SetPositionAndResetMotion_(Object_GetById(8), 0x1380000, 0x200000,
                                      0x1d80000);
    Func_02005178_(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Test(0x303)) {
    Object_SetPositionAndResetMotion_(Object_GetById(8), 0x1580000, 0x200000,
                                      0x1d80000);
    Func_02005178_(0, 4, 3, 3, 19, 28);
  }
  Func_020053b8(gData_020054e0);
  object = Object_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->collisionFlags = 0;
  Func_02005338(0);
  if (Flag_IsSet(0x92d)) {
    Func_02005170(41, 0, 20, 0, 1, 4);
    Func_02005230_(11, 0, 0);
    Func_02005230_(12, 0, 0);
    Func_02005230_(13, 0, 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    if (gPartyState.entrance == 2) {
      Object_GetById(gPartyState.currentOwner)->y = 0x400000;
    } else if (gPartyState.entrance == 3 && Flag_IsSet(0x92d) == 0) {
      Func_02005230_(11, 0, 0);
      Func_02005230_(12, 0, 0);
      Func_02005230_(13, 0, 0);
      Func_02005028();
    }
  }
#if LANGUAGE != LANGUAGE_JAPANESE
  if (gPartyState.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}
