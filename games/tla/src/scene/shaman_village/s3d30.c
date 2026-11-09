#include "shaman_village.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_0200482c(void);

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

void Func_02003d30(void) {
  s32 angle;

  GameFlag_ClearBit(0x11b);
  Func_020048c4();
  Event_ResolvePendingActions(0);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  ((struct ShamanVillageObject *)Actor_GetById(15))->facing = 0xc000;
  FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(182), 0);
  wait(1);
  Func_0200482c();
  wait(1);
  angle = 0x4000;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPositionAndFacing(16, PIXELS(184), PIXELS(128), angle);
  Actor_SetPositionAndFacing(17, PIXELS(264), PIXELS(128), angle);
  Actor_SetSpeed(16, 0x19999, 0xcccc);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Camera_SetSpeedParameters(0xcccc, 0x1999);
  Camera_MoveWithinMapBounds(PIXELS(224), -1, PIXELS(280), 1);
  Actor_WalkTo(16, 184, 256);
  Actor_WalkToAndWait(17, 264, 256);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_WalkTo(16, 208, 264);
  Actor_SetPositionAndReset(17, 240, 264);
  Actor_SetModeById(16, 1);
  Event_Wait(10);
  Actor_ArmCallback(16, angle, 0);
  Actor_ArmCallback(17, angle, 0);
  Actor_FaceDirectionAndWait(15, FACING_WEST);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Func_020049f4();
  Event_SetMessageId((s32)MsgShamanMustFinishTrialRoad);
  Event_ShowMessage(0x200f, 0);
  Actor_SetModeById(15, 3);
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_OpenMessage(0x200f, 0);
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
      Motion_SetModeAndWaitAnimation(15, 3);
      Sound_Play(76);
      Event_ShowMessageAndWait(0x200f, 0);
      GameFlag_SetBit(0x200);
      Func_02001380();
      return;
    }
    EVENT_WORK->field1c4++;
  } else {
    EVENT_WORK->field1c4 += 2;
  }
  Event_OpenMessage(0x200f, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200290c();
  }
  Func_02004a2c();
  GameFlag_ClearBit(0x12f);
  Func_020048cc();
}
