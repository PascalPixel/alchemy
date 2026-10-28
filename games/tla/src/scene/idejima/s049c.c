#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Engine_EventStart(void);
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorSetAnimation();
void Engine_AudioPlayCue(s32 cue);
s32 Engine_ActorShowEmote();
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_PrepareObjectAndApplyValue();
void FieldEvent_WaitFrames();
void Motion_SetVarCbAndRefresh();
void Motion_SetModeAndWaitAnimation();
void Object_TurnToFaceEachOther();
void ObjectMotion_Launch();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetAngleToward();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02003190();
void Func_02003228();
void Func_02003298();
void Func_02003310(s32 skip);
void Func_020032e8(s32 countdown);
void Engine_ActorSetAttachedEffect();
void Engine_CameraSetSpeed();
void Camera_MoveWithinMapBounds();
void ObjectMotion_EnableActionAndSetCallback();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void FieldMessage_Show();
void Task_Wait();
void Func_02000098();

extern const u8 gData_02003650[];
extern const u8 gData_02003734[];
extern const u8 gData_02003790[];

/// A scene of actor 6 and the leader that ends by setting flag 0x865.
void Func_0200049c(void) {
  struct ObjectRuntime *leader;

  Engine_EventStart();
  Func_02003310(0);
  Motion_SetVarCbAndRefresh(6, 2);
  Engine_EventSetMessage(MsgIdejimaSecondGroan);
  Func_02003298(6, 0, 40);
  Engine_EventShowMessage(6, 0);
  Engine_ActorSetAnimation(6, 18);
  Engine_EventShowMessage(6, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 14, 40);
  Object_GetById(6)->motionFlags = 3;
  ObjectMotion_Launch(6, 4, 0);
  Engine_ActorSetAnimation(6, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 1);
  FieldEvent_WaitFrames(20);
  Object_TurnToFaceEachOther(6, gAdventure.currentOwner, 0);
  Engine_EventShowMessage(6, 0);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  Engine_ActorShowEmote(6, 0x105, 20);
  Engine_EventShowMessage(6, 0);
  Call3(ObjectMotion_ArmCallback, 6, 0xe000, 20);
  Call3(ObjectMotion_ArmCallback, 6, 0xa000, 40);
  Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
  Engine_ActorShowEmote(6, 0x100, 20);
  Engine_EventShowMessage(6, 0);
  ObjectMotion_SetAngleToward(6, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(20);
  Engine_ActorSetAnimation(6, 3);
  Engine_EventShowMessage(6, 0);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  Engine_EventShowMessage(6, 0);
  Call3(ObjectMotion_SetSpeedParameters, 6, 0x19999, 0xcccc);
  Call3(ObjectMotion_SetPositionAndReset, 6, 212, 680);
  Call3(ObjectMotion_ArmCallback, 6, 0xe000, 20);
  Engine_EventShowMessage(6, 0);
  Engine_ActorSetAnimation(6, 2);
  leader = Object_GetById(gAdventure.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(6, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(6);
  Func_02003228(6, 0, 0);
  Event_PrepareObjectAndApplyValue(6, 1);
  GameFlag_SetBit(0x865);
  Engine_EventEnd();
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
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02000640(void) {
  Engine_EventStart();
  Func_02003310(0);
  Engine_AudioPlayCue(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020032e8(3);
}

void Func_02000664(void) {
  if (!FlagCheck(0x200)) {
    ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0xcccc, 0x6666);
    ActorPlace(gAdventure.currentOwner, 152, 756);
    FlagSet(0x200);
  }
}

/// The three scenes of actors 8, 5 and 6 with the leader, each run once:
/// flags 0x863, 0x864 and 0x865 record them.
void Func_020006a8(void) {
  struct ObjectRuntime *leader;

  if (FlagCheck(0x863) == 0) {
    Engine_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call1(Engine_EventSetMessage, (s32)MsgIdejimaHeyFelix);
    Call2(Engine_EventShowMessage, 8, 0);
    Call3(Func_02003228, 5, 0, 0);
    Call3(Func_02003228, 6, 0, 0);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, 0xa80000, -1, 0x2d10000, 1);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
    Call6(Func_02003190, 38, 31, 2, 1, 25, 42);
    Call1(Func_02000098, 8);
    Call3(Func_02003228, 8, 0x1380000, 0x29a0000);
    Call1(Task_Wait, 1);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call3(ObjectMotion_SetPositionAndReset, 8, 212, 680);
    Call3(ObjectMotion_SetPositionAndReset, 8, 192, 712);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call1(Object_RefreshSelectorById, 8);
    Call2(Object_SetActionCallbackAndRefreshById, 8, (s32)gData_02003650);
    Call3(ObjectMotion_Launch, 8, 4, 20);
    Call2(Engine_EventShowMessage, 8, 0);
    Value3(Engine_ActorShowEmote, 8, 257, 0);
    Call2(Engine_EventShowMessage, 8, 0);
    Value3(Engine_ActorShowEmote, 8, 0x100, 20);
    Call2(Engine_EventShowMessage, 8, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0xd000, 0);
    Call2(Engine_EventShowMessage, 8, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
    Call2(Engine_EventShowMessage, 8, 0);
    Call2(Engine_ActorSetAnimation, 8, 4);
    Call2(Engine_EventShowMessage, 8, 0);
    Call3(ObjectMotion_SetPositionAndReset, 8, 172, 756);
    Call3(Object_TurnToFaceEachOther, 8, gAdventure.currentOwner, 0);
    Call2(Engine_ActorSetAnimation, 8, 3);
    Call2(Engine_EventShowMessage, 0x6008, 0);
    Call2(Engine_ActorSetAnimation, 8, 2);
    leader = Object_GetById(gAdventure.currentOwner);
    if (leader != 0)
      ObjectMotion_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                                       ((s16 *)&leader->z)[1]);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 8);
    Call3(Func_02003228, 8, 0, 0);
    Call2(FieldMessage_Show, (s32)MsgIdejimaHeyFelix + 8, 1);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0x4000, 20);
    FlagSet(0x863);
    Engine_EventEnd();
  }
  if (FlagCheck(0x864) == 0) {
    Engine_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call3(Func_02003228, 5, 0x1380000, 0x29a0000);
    Call1(Task_Wait, 1);
    Call1(Engine_EventSetMessage, (s32)MsgIdejimaCantBelieveYouTwo);
    Call2(Engine_EventShowMessage, 5, 0);
    Call3(Func_02003228, 6, 0, 0);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, 0xa80000, -1, 0x2d10000, 1);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call2(ObjectMotion_EnableActionAndSetCallback, 8, (s32)gData_02003734);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
    Call6(Func_02003190, 38, 31, 2, 1, 17, 46);
    Call1(Func_02000098, 5);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0x19999, 0xcccc);
    Call3(ObjectMotion_SetPositionAndReset, 5, 212, 680);
    Call3(ObjectMotion_SetPositionAndReset, 5, 192, 712);
    Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
    Call1(Object_RefreshSelectorById, 5);
    Value3(Engine_ActorShowEmote, 5, 259, 0);
    Call3(ObjectMotion_Launch, 5, 4, 20);
    Call2(Engine_EventShowMessage, 5, 0);
    Call2(Engine_EventShowMessage, 5, 0);
    Call2(Engine_ActorSetAttachedEffect, gAdventure.currentOwner, 258);
    Call2(Engine_ActorSetAttachedEffect, 8, 258);
    Value3(Engine_ActorShowEmote, 5, 263, 20);
    Call2(Engine_EventShowMessage, 5, 0);
    Value3(Engine_ActorShowEmote, 8, 261, 0);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
    Call3(Func_02003298, 8, 0, 40);
    Value3(Engine_ActorShowEmote, 5, 261, 40);
    Call2(Engine_EventShowMessage, 5, 0);
    Call3(ObjectMotion_ArmCallback, 5, 0xe000, 40);
    Call2(Engine_EventShowMessage, 0x4005, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xa000, 20);
    Call2(Engine_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
    Call3(ObjectMotion_SetPositionAndReset, 5, 178, 728);
    Call3(ObjectMotion_ArmCallback, 5, 0x6000, 0);
    Call2(Engine_EventShowMessage, 0x4005, 0);
    Call3(ObjectMotion_ArmCallback, 8, 0xd000, 0);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 20);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
    Call2(ObjectMotion_EnableActionAndSetCallback, 8, (s32)gData_02003790);
    Call2(Object_SetActionCallbackAndRefreshById, 5, (s32)gData_02003790);
    Call2(Event_PrepareObjectAndApplyValue, 5, 1);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0x4000, 20);
    FlagSet(0x864);
    Engine_EventEnd();
  }
  if (FlagCheck(0x865) == 0) {
    Engine_EventStart();
    Call1(Func_02003310, 0);
    Func_02000664();
    Call3(Func_02003228, 6, 0x1380000, 0x29a0000);
    Call1(Task_Wait, 1);
    Call1(Engine_EventSetMessage, (s32)MsgIdejimaWaitForMe);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Engine_CameraSetSpeed, 0x9999, 0x1333);
    Call4(Camera_MoveWithinMapBounds, 0xa80000, -1, 0x2d10000, 1);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0x19999, 0xcccc);
    Call2(ObjectMotion_EnableActionAndSetCallback, 8, (s32)gData_02003734);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
    Call6(Func_02003190, 38, 31, 2, 1, 14, 40);
    Object_GetById(6)->motionFlags = 3;
    Call3(ObjectMotion_Launch, 6, 4, 0);
    Call2(Engine_ActorSetAnimation, 6, 1);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 1);
    Call1(FieldEvent_WaitFrames, 20);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0x19999, 0xcccc);
    Call3(ObjectMotion_SetPositionAndReset, 6, 212, 680);
    Call3(ObjectMotion_SetPositionAndReset, 6, 192, 712);
    Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
    Call1(Object_RefreshSelectorById, 6);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Engine_ActorSetAttachedEffect, gAdventure.currentOwner, 258);
    Value3(Engine_ActorShowEmote, 6, 263, 20);
    Call2(Engine_EventShowMessage, 6, 0);
    Call3(Func_02003298, 8, 0, 40);
    Call2(Engine_EventShowMessage, 6, 0);
    Value3(Engine_ActorShowEmote, gAdventure.currentOwner, 265, 40);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Engine_ActorSetAnimation, 6, 4);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Engine_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
    Call3(ObjectMotion_ArmCallback, 6, 0x8000, 40);
    Call3(ObjectMotion_ArmCallback, 6, 0x2000, 40);
    Call3(ObjectMotion_ArmCallback, 6, 0x6000, 0);
    Call2(Engine_EventShowMessage, 6, 0);
    Value3(Engine_ActorShowEmote, 6, 257, 20);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 4);
    Call2(Motion_SetModeAndWaitAnimation, 8, 4);
    Call2(Engine_EventShowMessage, 8, 0);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call2(Engine_EventShowMessage, 6, 0);
    Call2(Engine_ActorSetAnimation, 8, 3);
    Call2(Motion_SetModeAndWaitAnimation, 8, 3);
    Call2(ObjectMotion_EnableActionAndSetCallback, 8, (s32)gData_02003790);
    Call2(Object_SetActionCallbackAndRefreshById, 6, (s32)gData_02003790);
    Call2(Event_PrepareObjectAndApplyValue, 6, 1);
    Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0x4000, 20);
    FlagSet(0x865);
    Engine_EventEnd();
  }
}
