#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020010dc();
void Func_020010e4();

void Func_02000d68();
void Func_02000d28();

void Func_02000e20(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_CommitPositionAndActivate(4, 16, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e44(void) {
  Actor_WalkByAndWait(4, 0, -14);
  Actor_CommitPositionAndActivate(4, -16, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e6c(void) {
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Func_02000d68();
  return;
}

void Func_02000e88(s32 arg0) {
  if (arg0 != 1)
    goto L_02008e9c;
  Func_02000d28();
  GameFlag_SetBit(0x9af);
L_02008e9c:
  return;
}

void Func_02000ea0(void) {
  s32 result0;

  result0 = Event_GetFlag(0x917);
  if (result0 == 0)
    goto L_02008eee;
  MapGrid_CopyCellAttributes(31, 43, 9, 12, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
L_02008eee:
  return;
}

void Func_02000ef4(void) {
  Func_020010dc();
  Event_ResolvePendingActions(0);
  MapGrid_CopyCellAttributes(31, 43, 9, 11, 48, 47);
  MapGrid_CopyCellsTo(30, 110, 48, 110, 9, 13);
  MapGrid_CopyCellAttributes(52, 110, 1, 1, 52, 111);
  GameFlag_SetBit(0x917);
  Func_020010e4();
  return;
}
