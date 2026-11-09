#include "local.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_02000438(void) {
  s32 sound;
  s32 drop;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65dRoom1) {
    sound = 12;
    drop = 17;
  } else {
    sound = 3;
    drop = 9;
  }
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, drop);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(sound);
  Func_02000650();
}
