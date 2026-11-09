#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "macros.h"
#include "event_inline.h"

extern char SceneId_Ov676Room1;

void Func_020023c8(void);

void Func_02002244(void) {
  if (GameFlag_Test(0x300)) {
    SET_POSITION(Actor_GetById(9), 0xa80000, 0, 0xc00000);
  }
}

s32 Func_02002278(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1) {
    if (gMainRoutineWork.entrance == 15)
      Func_020023c8();
    else
      Func_02002244();
  }
  return 0;
}

s32 Func_020022b0(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1 && GameFlag_Test(0x300))
    MapGrid_CopyCellAttributes(8, 12, 1, 1, 10, 12);
  return 0;
}

void Func_020022f4(void) {}
