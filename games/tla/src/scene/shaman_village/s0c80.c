#include "shaman_village.h"

extern u32 gData_02005aa0[];
extern u32 gData_02005aac[];
extern u32 gData_02005bc0[];
extern u32 gData_02005ca4[];
extern u32 gData_02005f98[];
extern u32 gData_020061f0[];
extern u32 gData_020062d4[];

void *Func_02000c80(void) {
  s32 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_ShamanVillage1)
    return gData_02005aac;
  if (scene == (s32)&SceneId_ShamanVillage3)
    return gData_02005bc0;
  if (scene == (s32)&SceneId_ShamanVillage2)
    return gData_02005ca4;
  if (scene == (s32)&SceneId_ShamanVillage4)
    return gData_02005f98;
  if (scene == (s32)&SceneId_ShamanVillage5)
    return gData_020061f0;
  if (scene == (s32)&SceneId_ShamanVillage6)
    return gData_020062d4;
  return gData_02005aa0;
}
