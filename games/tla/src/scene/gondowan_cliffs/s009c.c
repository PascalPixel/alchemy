#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_0200178c[];
extern const u8 gData_0200193c[];

const u8 *Func_0200009c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov66bRoom2)
    return gData_0200193c;
  return gData_0200178c;
}
