#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern u8 gData_020054e0[];
extern u8 gData_020054ec[];
extern u8 gData_02005870[];
extern u8 gData_02005a44[];
extern u8 gData_02005e70[];
extern u8 gData_02005ed0[];

u8 *Func_02000874(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov694Room1)
    return gData_020054ec;
  if (scene == (s32)&SceneId_Ov694Room4)
    return gData_02005870;
  if (scene == (s32)&SceneId_Ov694Room2) {
    if (gMainRoutineWork.entrance == 4)
      return gData_02005e70;
    return gData_02005a44;
  }
  if (scene == (s32)&SceneId_Ov694Room3)
    return gData_02005ed0;
  return gData_020054e0;
}
