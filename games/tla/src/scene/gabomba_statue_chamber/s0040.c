#include "gabomba_statue_chamber.h"

u8 *Func_02000040(void) {
  s16 scene = gAdventure.scene;

  if (scene == (s32)&SceneId_Ov672Room1)
    return gData_0200a08c;
  return 0;
}
