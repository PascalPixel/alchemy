#include "scene_tables.h"
#include "ankohl_ruins.h"
#include "task.h"

void Func_020020fc(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    s32 found = Func_02002dcc(Func_02001f7c);

    if (found == -1) {
      Func_02002e44();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(207);
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(392), 1);
      Func_02002eb4();
      Task_AddFrame(Func_02001f7c, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Func_02002eb4();
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Func_02002e4c();
    }
  }
}

void Func_020021d4(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    void (*func)(void) = Func_02001f7c;

    if (Func_02002dcc(func) == -1) {
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      Task_AddFrame(func, TASK_PASS_VBLANK);
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Func_02002e4c();
    }
  }
}
