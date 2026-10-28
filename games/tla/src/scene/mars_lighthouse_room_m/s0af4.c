#include "mars_lighthouse_room_m.h"

const u8 *Func_02000af4(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov6ACRoom6)
    return gData_020036c8;
  if (scene == (s32)&SceneId_Ov6ACRoom1)
    return gData_02003708;
  if (scene == (s32)&SceneId_Ov6ACRoom2 || scene == (s32)&SceneId_Ov6ACRoom3 ||
      scene == (s32)&SceneId_Ov6ACRoom4 || scene == (s32)&SceneId_Ov6ACRoom5)
    return gData_02003858;
  return gData_020036b8;
}
