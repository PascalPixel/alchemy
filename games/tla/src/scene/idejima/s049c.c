#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Import_EventStart(void);
void Import_EventEnd();
void Import_EventSetMessageId();
void Import_EventShowMessageAndWait();
void Import_ActorSetAnimation();
void Import_SoundPlay(s32 cue);
s32 Import_ActorShowEmote();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_PrepareObjectAndApplyValue();
void FieldEvent_Wait();
void Motion_SetVarCbAndRefresh();
void Motion_SetModeAndWaitAnimation();
void Actor_TurnToFaceEachOther();
void Actor_Jump();
void Actor_ArmCallback();
void Actor_SetAngleToward();
void Actor_ResetAndSetPosition();
void Actor_WaitForMoveById();
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003190();
void Func_02003228();
void Func_02003298();
void Func_02003310(s32 skip);
void Func_020032e8(s32 countdown);
void Import_ActorSetAttachedEffect();
void Import_CameraSetSpeed();
void Camera_MoveWithinMapBounds();
void Actor_SetMovementTypeById();
void Actor_RefreshSelectorById();
void Actor_SetMovementTypeAndWaitById();
void FieldMessage_Show();
void Task_Wait();
void Func_02000098();

extern const u8 gData_02003650[];
extern const u8 gData_02003734[];
extern const u8 gData_02003790[];

/// A scene of actor 6 and the leader that ends by setting flag 0x865.
void Func_0200049c(void) {
  struct Actor *leader;

  Import_EventStart();
  Func_02003310(0);
  Motion_SetVarCbAndRefresh(6, 2);
  Import_EventSetMessageId(MsgIdejimaSecondGroan);
  Func_02003298(6, 0, 40);
  Import_EventShowMessageAndWait(6, 0);
  Import_ActorSetAnimation(6, 18);
  Import_EventShowMessageAndWait(6, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 14, 40);
  Actor_GetById(6)->motionFlags = 3;
  Actor_Jump(6, 4, 0);
  Import_ActorSetAnimation(6, 1);
  Actor_SetSingleChildFlags(Actor_GetById(6), 1);
  FieldEvent_Wait(20);
  Actor_TurnToFaceEachOther(6, gMainRoutineWork.currentOwner, 0);
  Import_EventShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Import_ActorShowEmote(6, 0x105, 20);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 6, FACING_NORTHEAST, 20);
  Call3(Actor_ArmCallback, 6, FACING_NORTHWEST, 40);
  Call3(Actor_ArmCallback, 6, FACING_SOUTHWEST, 0);
  Import_ActorShowEmote(6, 0x100, 20);
  Import_EventShowMessageAndWait(6, 0);
  Actor_SetAngleToward(6, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  Import_ActorSetAnimation(6, 3);
  Import_EventShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_SetSpeedParameters, 6, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 6, 212, 680);
  Call3(Actor_ArmCallback, 6, FACING_NORTHEAST, 20);
  Import_EventShowMessageAndWait(6, 0);
  Import_ActorSetAnimation(6, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(6, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Func_02003228(6, 0, 0);
  Event_PrepareObjectAndApplyValue(6, 1);
  GameFlag_SetBit(0x865);
  Import_EventEnd();
}

static inline s32 FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void ActorPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(actor, x, z);
}

void Func_02000640(void) {
  Import_EventStart();
  Func_02003310(0);
  Import_SoundPlay(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020032e8(3);
}

void Func_02000664(void) {
  if (!FlagCheck(0x200)) {
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    ActorPlace(gMainRoutineWork.currentOwner, 152, 756);
    FlagSet(0x200);
  }
}

/// The three scenes of actors 8, 5 and 6 with the leader, each run once:
/// flags 0x863, 0x864 and 0x865 record them.
void Func_020006a8(void) {
  struct Actor *leader;

  if (FlagCheck(0x863) == 0) {
    Import_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call1(Import_EventSetMessageId, (s32)MsgIdejimaHeyFelix);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call3(Func_02003228, 5, 0, 0);
    Call3(Func_02003228, 6, 0, 0);
    Call2(Import_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, PIXELS(168), -1, PIXELS(721), 1);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHEAST,
          0);
    Call6(Func_02003190, 38, 31, 2, 1, 25, 42);
    Call1(Func_02000098, 8);
    Call3(Func_02003228, 8, PIXELS(312), PIXELS(666));
    Call1(Task_Wait, 1);
    Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 8, 212, 680);
    Call3(Actor_SetPositionAndReset, 8, 192, 712);
    Call3(Actor_ArmCallback, 8, FACING_SOUTH + FACING_STEP, 0);
    Call1(Actor_RefreshSelectorById, 8);
    Call2(Actor_SetMovementTypeAndWaitById, 8, (s32)gData_02003650);
    Call3(Actor_Jump, 8, 4, 20);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Value3(Import_ActorShowEmote, 8, 257, 0);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Value3(Import_ActorShowEmote, 8, 0x100, 20);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call3(Actor_ArmCallback, 8, FACING_NORTH + FACING_STEP, 0);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call3(Actor_ArmCallback, 8, FACING_SOUTH + FACING_STEP, 0);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call2(Import_ActorSetAnimation, 8, 4);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call3(Actor_SetPositionAndReset, 8, 172, 756);
    Call3(Actor_TurnToFaceEachOther, 8, gMainRoutineWork.currentOwner, 0);
    Call2(Import_ActorSetAnimation, 8, 3);
    Call2(Import_EventShowMessageAndWait, 0x6008, 0);
    Call2(Import_ActorSetAnimation, 8, 2);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != 0)
      Actor_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                                ((s16 *)&leader->z)[1]);
    Call1(Actor_WaitForMoveById, 8);
    Call3(Func_02003228, 8, 0, 0);
    Call2(FieldMessage_Show, (s32)MsgIdejimaHeyFelix + 8, 1);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    FlagSet(0x863);
    Import_EventEnd();
  }
  if (FlagCheck(0x864) == 0) {
    Import_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call3(Func_02003228, 5, PIXELS(312), PIXELS(666));
    Call1(Task_Wait, 1);
    Call1(Import_EventSetMessageId, (s32)MsgIdejimaCantBelieveYouTwo);
    Call2(Import_EventShowMessageAndWait, 5, 0);
    Call3(Func_02003228, 6, 0, 0);
    Call2(Import_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, PIXELS(168), -1, PIXELS(721), 1);
    Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call2(Actor_SetMovementTypeById, 8, (s32)gData_02003734);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHEAST,
          0);
    Call6(Func_02003190, 38, 31, 2, 1, 17, 46);
    Call1(Func_02000098, 5);
    Call3(Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 5, 212, 680);
    Call3(Actor_SetPositionAndReset, 5, 192, 712);
    Call3(Actor_ArmCallback, 5, FACING_SOUTHWEST, 0);
    Call1(Actor_RefreshSelectorById, 5);
    Value3(Import_ActorShowEmote, 5, 259, 0);
    Call3(Actor_Jump, 5, 4, 20);
    Call2(Import_EventShowMessageAndWait, 5, 0);
    Call2(Import_EventShowMessageAndWait, 5, 0);
    Call2(Import_ActorSetAttachedEffect, gMainRoutineWork.currentOwner, 258);
    Call2(Import_ActorSetAttachedEffect, 8, 258);
    Value3(Import_ActorShowEmote, 5, 263, 20);
    Call2(Import_EventShowMessageAndWait, 5, 0);
    Value3(Import_ActorShowEmote, 8, 261, 0);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
    Call3(Func_02003298, 8, 0, 40);
    Value3(Import_ActorShowEmote, 5, 261, 40);
    Call2(Import_EventShowMessageAndWait, 5, 0);
    Call3(Actor_ArmCallback, 5, FACING_NORTHEAST, 40);
    Call2(Import_EventShowMessageAndWait, 0x4005, 0);
    Call3(Actor_ArmCallback, 8, FACING_SOUTHEAST + FACING_STEP, 0);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHWEST,
          20);
    Call2(Import_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
    Call3(Actor_SetPositionAndReset, 5, 178, 728);
    Call3(Actor_ArmCallback, 5, FACING_SOUTHWEST, 0);
    Call2(Import_EventShowMessageAndWait, 0x4005, 0);
    Call3(Actor_ArmCallback, 8, FACING_NORTH + FACING_STEP, 0);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHEAST,
          20);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
    Call2(Actor_SetMovementTypeById, 8, (s32)gData_02003790);
    Call2(Actor_SetMovementTypeAndWaitById, 5, (s32)gData_02003790);
    Call2(Event_PrepareObjectAndApplyValue, 5, 1);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    FlagSet(0x864);
    Import_EventEnd();
  }
  if (FlagCheck(0x865) == 0) {
    Import_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call3(Func_02003228, 6, PIXELS(312), PIXELS(666));
    Call1(Task_Wait, 1);
    Call1(Import_EventSetMessageId, (s32)MsgIdejimaWaitForMe);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Import_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, PIXELS(168), -1, PIXELS(721), 1);
    Call3(Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call2(Actor_SetMovementTypeById, 8, (s32)gData_02003734);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTHEAST,
          0);
    Call6(Func_02003190, 38, 31, 2, 1, 14, 40);
    Actor_GetById(6)->motionFlags = 3;
    Call3(Actor_Jump, 6, 4, 0);
    Call2(Import_ActorSetAnimation, 6, 1);
    Actor_SetSingleChildFlags(Actor_GetById(6), 1);
    Call1(FieldEvent_Wait, 20);
    Call3(Actor_SetSpeedParameters, 6, 0x19999, 0xcccc);
    Call3(Actor_SetPositionAndReset, 6, 212, 680);
    Call3(Actor_SetPositionAndReset, 6, 192, 712);
    Call3(Actor_ArmCallback, 6, FACING_SOUTHWEST, 0);
    Call1(Actor_RefreshSelectorById, 6);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Import_ActorSetAttachedEffect, gMainRoutineWork.currentOwner, 258);
    Value3(Import_ActorShowEmote, 6, 263, 20);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call3(Func_02003298, 8, 0, 40);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Value3(Import_ActorShowEmote, gMainRoutineWork.currentOwner, 265, 40);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Import_ActorSetAnimation, 6, 4);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Import_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 3);
    Call3(Actor_ArmCallback, 6, FACING_WEST, 40);
    Call3(Actor_ArmCallback, 6, FACING_SOUTHEAST, 40);
    Call3(Actor_ArmCallback, 6, FACING_SOUTHWEST, 0);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Value3(Import_ActorShowEmote, 6, 257, 20);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Motion_SetModeAndWaitAnimation, gMainRoutineWork.currentOwner, 4);
    Call2(Motion_SetModeAndWaitAnimation, 8, 4);
    Call2(Import_EventShowMessageAndWait, 8, 0);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call2(Import_EventShowMessageAndWait, 6, 0);
    Call2(Import_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, 8, 3);
    Call2(Actor_SetMovementTypeById, 8, (s32)gData_02003790);
    Call2(Actor_SetMovementTypeAndWaitById, 6, (s32)gData_02003790);
    Call2(Event_PrepareObjectAndApplyValue, 6, 1);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    FlagSet(0x865);
    Import_EventEnd();
  }
}
