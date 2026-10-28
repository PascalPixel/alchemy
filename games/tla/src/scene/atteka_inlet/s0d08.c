#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

void Func_02000dc4(void);
void Func_02000ed0(void);

s32 Func_02000d08(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov693Room1)
    Func_02000dc4();
  else if (scene == (s32)&SceneId_Ov693Room2)
    Func_02000ed0();
  return 0;
}
