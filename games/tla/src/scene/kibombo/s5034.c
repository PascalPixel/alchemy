#include "kibombo.h"

s32 Func_02005034(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    Func_02005114();
  else if (scene == (s32)&SceneId_Kibombo4)
    Func_0200516c();
  else if (scene == (s32)&SceneId_Kibombo2)
    Func_020054e0();
  else if (scene == (s32)&SceneId_Kibombo5)
    Func_02005594();
  else if (scene == (s32)&SceneId_Kibombo6)
    Func_020057e0();
  else if (scene == (s32)&SceneId_Kibombo3)
    Func_02005898();
  return 0;
}
