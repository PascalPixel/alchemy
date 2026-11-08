#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "version.h"
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
  if (GameFlag_Test(SPLIT_CONSTANT(0x300, 2))) {
    Actor_SetPositionAndResetMotion_(Actor_GetById(8), 0x1380000, 0x200000,
                                     0x1d80000);
    Func_02005178_(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Test(SPLIT_CONSTANT(0x204, 255))) {
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
  if (Flag_IsSet(SPLIT_CONSTANT(0x900, 45))) {
    {
      s32 e = 1;
      s32 f = 4;

      Func_02005170(41, 0, 20, 0, e, f);
    }
    Func_02005230(11, 0, 0);
    Func_02005230(12, 0, 0);
    Func_02005230(13, 0, 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    if (gMainRoutineWork.entrance == 2) {
      Actor_GetById(gMainRoutineWork.currentOwner)->y = 0x400000;
    } else if (gMainRoutineWork.entrance == 3 && Flag_IsSet(SPLIT_CONSTANT(0x900, 45)) == 0) {
      Func_02005230(11, 0, 0);
      Func_02005230(12, 0, 0);
      Func_02005230(13, 0, 0);
      Func_02005028();
    }
  }
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
#endif
}
