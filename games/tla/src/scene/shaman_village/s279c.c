#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_0200279c(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowMeSomething);
  Event_ShowMessageAndWait(15, 0);
  Func_020048cc();
}

void Func_020027c0(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanCannotBuyGift);
  Event_ShowMessageAndWait(16, 0);
  Func_020048cc();
}

void Func_020027e4(void) {
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgShamanShowItToMoapa);
  Event_ShowMessageAndWait(17, 0);
  Func_020048cc();
}

void Func_02002808(void) {
  if (GameFlag_Test(0x9fe)) {
    Sound_Play(0);
    Func_020048c4();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgShamanFortuneGreeting);
    Actor_SetModeById(17, 5);
    Event_ShowMessageAndWait(17, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
    Func_020011f8();
    Actor_SetModeById(17, 1);
    GameFlag_SetBit(0x203);
    Func_020048cc();
  }
}

void Func_02002878(void) {
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 2, 17);
  GameFlag_SetBit(0x205);
}

void Func_0200289c(void) {
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 304);
  Actor_FaceDirection(16, FACING_SOUTH, 0);
  Actor_FaceDirection(17, FACING_SOUTH, 0);
  Actor_FaceDirection(15, FACING_SOUTH, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 168, 368);
  GameFlag_ClearBit(0x201);
}
