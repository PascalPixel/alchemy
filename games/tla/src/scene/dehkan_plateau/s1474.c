#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

void Func_02001514(s32 show, s32 first, u32 count);

void Func_02001474(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Func_02001514(show, 13, 14);
  else if (scene == (s32)&SceneId_Ov655Room2)
    Func_02001514(show, 25, 13);
  else if (scene == (s32)&SceneId_Ov655Room3)
    Func_02001514(show, 26, 10);
  else if (scene == (s32)&SceneId_Ov655Room4)
    Func_02001514(show, 8, 3);
  else if (scene == (s32)&SceneId_Ov655Room6)
    Func_02001514(show, 10, 6);
}
