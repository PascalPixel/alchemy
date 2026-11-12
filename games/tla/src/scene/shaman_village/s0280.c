#include "shaman_village.h"

void Func_02000280(void) { Func_080d7788(18, 3, 13); }

const struct SceneEntrance *Func_02000290(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020052b4;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_02005314;
  return gData_02005284;
}
