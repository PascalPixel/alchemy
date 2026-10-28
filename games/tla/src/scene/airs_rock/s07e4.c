#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;
extern s32 gData_020043d4[];
extern s32 gData_020044d0[];
extern s32 gData_0200459c[];

s32 *Func_020007e4(void) {
  s32 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov663Room1)
    return gData_020043d4;
  if (scene == (s32)&SceneId_Ov663Room2)
    return gData_020044d0;
  return gData_0200459c;
}
