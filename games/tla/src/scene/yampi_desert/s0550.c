#include "scene.h"

extern const u8 MsgAsYouCan[];

void Func_02000550(void) {
  Func_02004040();
  Func_020041b8(0);
  if (!Scene_FlagTest(0x88f))
    Func_020004e0();
  Actor_SetAngleToward(12, gMainRoutineWork.currentOwner, 0);
  Func_020040c8((s32)MsgAsYouCan);
  Func_020040d0(12, 0);
  Scene_FlagSet(0x88f);
  Func_02004048();
}
