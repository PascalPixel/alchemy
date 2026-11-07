#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"

void GameFlag_SetBit(s32 flag);

void Func_02001264(s32 a, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    GameFlag_SetBit(id + 0x97f);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    GameFlag_SetBit(id + 0x995);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    GameFlag_SetBit(id + 0x996);
}
