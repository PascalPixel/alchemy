#include "local.h"

const u8 *Func_02000344(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02002e30;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_02002e60;
  return 0;
}
