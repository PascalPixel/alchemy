#include "types.h"

#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

s32 Func_02003494(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room7 &&
      gMainRoutineWork.entrance == 1 && !GameFlag_Test(0x109)) {
    GameFlag_ClearBit(0x8cd);
    GameFlag_ClearBit(0x8ce);
    GameFlag_ClearBit(0x8cf);
  }
  return 0;
}
