#include "local.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02004464();
void Func_0200457c(void);
s32 Func_0200446c();
extern u8 MsgProx80[];

void Func_02001cf0(void) {
  s32 t83;
  s32 t144;
  s32 t159;
  s32 t202;
  s32 t207;
  s32 angleC000;
  s32 angleB000;
  s32 angle3000;

  Func_02004464();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_Stop(0x14);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(184), PIXELS(484));
  /* FAKEMATCH(temporary): the three facings held in locals set before their
   * first placement; as constants, each placement rebuilds them, the later
   * turns cannot share them and 76 instructions differ. */
  angleC000 = 0xc000;
  angleB000 = 0xb000;
  Actor_SetPositionAndFacing(0, PIXELS(168), PIXELS(484), angleC000);
  Actor_SetPositionAndFacing(27, PIXELS(200), PIXELS(484), angleB000);
  angle3000 = 0x3000;
  Actor_SetPositionAndFacing(17, PIXELS(138), PIXELS(412), angle3000);
  Actor_SetPositionAndFacing(20, PIXELS(220), PIXELS(600), angleB000);
  Actor_SetPosition(5, PIXELS(184), PIXELS(684));
  System_Wait(1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(448), 1);
  Func_0200457c();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgProx80);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(26, 0x5000, 0);
  Prox_Jump(26, 0x108, 20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(0, 0, 20);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x108, 0);
  Prox_Jump(0, 0x108, 40);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(19, 4);
  Event_ShowMessage(0x4013, 0);
  Prox_Jump(0, 0x109, 20);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(27, FACING_WEST, 20);
  Prox_Jump(27, 0x108, 0);
  Event_ShowMessage(27, 0);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Prox_Jump(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirectionAndWait(26, 0x3000);
  Actor_SetAnimation(26, 4);
  Event_ShowMessage(26, 0);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(0x13, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x4013, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(26, 0x5000, 0);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_ShowEmote(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_ShowEmote(25, 0x103, 20);
  Event_ShowMessage(25, 0);
  Actor_ShowEmote(27, 0x100, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(26, 0x3000, 0);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_ShowEmote(25, 0x105, 20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(26, 4);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0x14);
  Actor_FaceDirectionAndWait(0, 0);
  Actor_ShowEmote(0, 0x108, 0);
  Event_OpenMessage(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t83 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t83 == 0) {
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessage(26, 0);
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId++;
    Actor_SetAnimationAndWait(26, 4);
    Event_ShowMessage(26, 0);
  }
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Prox_Jump(25, 0x108, 20);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(27, 0xb000, 20);
  Actor_SetAnimationAndWait(27, 3);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirectionAndWait(0, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(0x4000, 0);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirectionAndWait(25, FACING_WEST);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Event_ShowMessage(25, 0);
  Prox_Jump(27, 0x109, 20);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0, 0);
  Actor_FaceDirection(0, 0, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Prox_Jump(gMainRoutineWork.currentOwner, 0x101, 0);
  Prox_Jump(0, 0x101, 0);
  Prox_Jump(26, 0x101, 0);
  Actor_ShowEmote(25, 0x101, 40);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(26, 3);
  Actor_SetAnimation(25, 3);
  Actor_FaceDirection(27, FACING_WEST, 0);
  Actor_ShowEmote(27, 0x108, 20);
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(26, 0x109, 20);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0x14);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(0x1b, 0x102);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(27, 0x5000);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirectionAndWait(0, 0);
  Prox_MessageOpen(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t144 = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t144 == 0) {
    FieldCamera_SetSpeed(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(512), 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(0x4000, 0);
    Event_ShowMessage(0x2005, 0);
    EVENT_WORK->messageId += 2;
  } else {
    EVENT_WORK->messageId += 2;
    FieldCamera_SetSpeed(0x26666, 0x4ccc);
    FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(512), 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 0x19999, 0xcccc);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(0x4000, 0);
    Event_ShowMessage(0x2005, 0);
  }
  t159 = ((s32)FieldCamera_GetActor());
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  FieldCamera_MoveTo(PIXELS(158), -1, PIXELS(432), 1);
  Func_0200457c();
  FieldActor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  FieldCamera_SetSpeed(0x13333, 0x2666);
  FieldCamera_MoveTo(PIXELS(172), -1, PIXELS(520), 1);
  FieldActor_WatchAndSetScript(0, 0x11);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, 0x11);
  FieldActor_WatchAndSetScript(0x1b, 0x11);
  Actor_SetSpeed(17, 0x19999, 0xcccc);
  Actor_WalkToAndWait(17, 140, 478);
  Actor_WalkToAndWait(17, 150, 540);
  Actor_WalkToAndWait(17, 160, 560);
  Actor_WalkToAndWait(17, 190, 640);
  FieldActor_Stop(0);
  FieldActor_Stop(gMainRoutineWork.currentOwner);
  FieldActor_Stop(0x1b);
  System_Wait(1);
  Actor_SetPosition(17, 0, 0);
  FieldEvent_Wait(10);
  FieldCamera_SetSpeed(0x26666, 0x4ccc);
  FieldCamera_MoveTo(PIXELS(184), -1, PIXELS(448), 1);
  Actor_WalkToAndWait(0, 168, 484);
  Actor_FaceDirectionAndWait(0, 0);
  Event_ShowMessage(0x4000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(0x401b, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_SetAnimationAndWait(25, 3);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(27, 3);
  Actor_SetSpeed(0, 0x19999, 0xcccc);
  Actor_SetSpeed(27, 0x19999, 0xcccc);
  Actor_SetAnimation(0, 2);
  t202 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t202 != 0) {
    Actor_SetDestination(0, FIELD_AT(t202, s16, 10),
                              FIELD_AT(t202, s16, 18));
  }
  FieldActor_WaitForMoveById(0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetAnimation(27, 2);
  t207 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t207 != 0) {
    Actor_SetDestination(27, FIELD_AT(t207, s16, 10),
                              FIELD_AT(t207, s16, 18));
  }
  FieldActor_WaitForMoveById(27);
  Actor_SetPosition(27, 0, 0);
  {
    struct EventWork *work = EVENT_WORK;

    work->screenEffect = 0x209;
    work->delay = 0x10;
  }
  Event_ClearFlag(0x12f);
  Event_SetFlag(0x9c5);
  Func_0200446c();
}
