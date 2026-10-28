#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern const u8 gData_02001ad8[];
extern const u8 gData_02001ae4[];
extern const u8 gData_02001c58[];
extern const u8 gData_02001db4[];

const u8 *Func_020005ec(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov66fRoom1)
    return gData_02001ae4;
  if (scene == (s32)&SceneId_Ov66fRoom2)
    return gData_02001c58;
  if (scene == (s32)&SceneId_Ov66fRoom3)
    return gData_02001db4;
  return gData_02001ad8;
}
