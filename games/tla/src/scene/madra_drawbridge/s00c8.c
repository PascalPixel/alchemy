#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000338(void);

void Func_020000c8(void) {
  if (Event_GetFlag(0x200) == 0) {
    Func_020002b8();
    Event_ResolvePendingActions(0);
    if (Event_GetFlag(0x301) == 0) {
      Camera_SetSpeedParameters(0x18000, 0x3000);
      Camera_MoveWithinMapBounds(PIXELS(164), -1, PIXELS(260), 1);
      Actor_WalkTo(gMainRoutineWork.currentOwner, 158, 316);
    }
    Actor_GetById(8)->actionFlags &= ~1;
    Actor_GetById(9)->actionFlags &= ~1;
    Actor_ResetAndSetPositionInMode2(9, 148, 252);
    Actor_SetPositionAndReset(8, 170, 252);
    Actor_WaitForMoveById(9);
    Actor_GetById(8)->actionFlags |= 1;
    Actor_GetById(9)->actionFlags |= 1;
    Actor_FaceDirection(8, FACING_SOUTH, 0);
    Actor_ArmCallback(9, FACING_SOUTH, 0);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    GameFlag_SetBit(0x200);
    Func_02000338();
    MapGrid_CopyCellAttributes(11, 9, 4, 1, 8, 14);
    if (Event_GetFlag(0x301) == 0) {
      Event_SetMessageId((s32)MsgMadraNeedEldersPermission);
      Event_Wait(10);
      Motion_SetVarCbAndRefresh(8, 1);
      Event_ShowMessageThenWait(8, 0, 5);
      Motion_SetModeAndWaitAnimation(9, 3);
      Event_ShowMessageAndWait(9, 0);
      GameFlag_SetBit(0x301);
      Func_020002c0();
    }
  }
}
