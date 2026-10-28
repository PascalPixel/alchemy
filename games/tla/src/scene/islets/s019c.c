#include "islets.h"

const u8 *Func_0200019c(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov674Room1)
    return gData_02004b20;
  if (scene == (s32)&SceneId_Ov674Room2)
    return gData_02004bb0;
  if (scene == (s32)&SceneId_Ov674Room3)
    return gData_02004ca0;
  if (scene == (s32)&SceneId_Ov674Room4)
    return gData_02004d84;
  if (scene == (s32)&SceneId_Ov674Room5)
    return gData_02004e5c;
  return gData_02004b14;
}
