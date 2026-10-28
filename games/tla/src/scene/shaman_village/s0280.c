#include "shaman_village.h"

void Func_02000280(void) { Func_02004a34(18, 3, 13); }

u8 *Func_02000290(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020052b4;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_02005314;
  return gData_02005284;
}
