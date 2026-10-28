#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_020023ec[];
extern const u8 gData_020023f8[];

const u8 *Func_0200013c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov657Room2)
    return gData_020023f8;
  return gData_020023ec;
}
