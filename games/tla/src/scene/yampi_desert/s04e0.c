#include "scene.h"

extern const u8 MsgCanTWe[];

void Func_020004e0(void) {
  Func_020040c8((s32)MsgCanTWe);
  Object_SetModeById(12, 4);
  Func_020040d0(12, 0);
  Scene_ArmCallback(13, 0x8000, 20);
  Object_SetModeById(13, 4);
  Func_020040d0(13, 0);
  Scene_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 20);
  Scene_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 0);
  Func_020040d0(13, 0);
}
