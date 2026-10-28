#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_020022cc[];
extern const u8 gData_0200235c[];

const u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room1)
    return gData_0200235c;
  return gData_020022cc;
}
