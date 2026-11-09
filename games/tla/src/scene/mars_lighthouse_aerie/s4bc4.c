#include "mars_lighthouse_aerie.h"
#include "scene_tables.h"

extern const u8 MsgWillYouCast[];

void Func_02003658(void);

void Func_02004bc4(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTH + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 480, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c2c(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(19, FACING_SOUTHEAST + FACING_STEP);
  FieldEvent_SetMessageId(MsgDoNotLet);
  Event_ShowMessage(0x8013, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 544, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c94(void) {
  Func_0200560c();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(18, PIXELS(512), PIXELS(232));
  System_Wait(1);
  FieldEvent_SetMessageId(MsgWillYouCast);
  Event_OpenMessage(18, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_SetPosition(18, 0, 0);
    System_Wait(1);
    Func_02003658();
  }
  Actor_SetPosition(18, 0, 0);
  System_Wait(1);
  Event_SetFlag(0x202);
  Func_02005614();
}
