#include "mars_lighthouse_room_m.h"

const u8 *Func_02000080(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6ACRoom6)
    return gData_02003508;
  if (scene == (s32)&SceneId_Ov6ACRoom1)
    return gData_02003598;
  return gData_020034f0;
}
