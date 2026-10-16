#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Task_Wait(s32 frames);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02000098(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  Engine_ActorSetAnimation(id, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 1);
  object->x -= 0xc0000;
  ((struct ActorSprite *)object->animation)->rotation = 0;
  Task_Wait(1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 1);
  object->velocityY = 0x40000;
  object->motionFlags = 3;
  Task_Wait(40);
}

void Engine_EventBegin();
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void GameFlag_SetBit();
void FieldEvent_WaitFrames();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_ArmCallback();
s32 Engine_ActorShowEmote();
void Engine_ActorSetAttachedEffect();
void Func_02003310();
void Func_02003190();
void Func_02003228();
void Func_02003298();
void Motion_SetVarCbAndRefresh();
void UiText_ShowPositionedMessageAndWait();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
void Event_PrepareObjectAndApplyValue();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();

// A scene of actor 8 and the leader that ends by setting flag 0x863.
void Func_020000f0(void) {
  s32 message;
  struct ObjectRuntime *actor;
  struct ObjectRuntime *leader;

  Engine_EventBegin();
  Func_02003310(0);
  Call2(Motion_SetVarCbAndRefresh, 8, 1);
  message = (s32)MsgIdejimaGroan;
  Engine_EventSetMessage(message);
  Call2(Engine_EventShowMessage, 8, 0);
  Call2(Motion_SetVarCbAndRefresh, 8, 2);
  Call2(Engine_EventShowMessage, 8, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 25, 42);
  actor = Object_GetById(8);
  Call2((void (*)())Engine_ActorSetAnimation, 8, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 1);
  Task_Wait(1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 1);
  actor->velocityY = 0x40000;
  actor->motionFlags = 3;
  Task_Wait(40);
  Call2(Engine_EventShowMessage, 8, 0);
  ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
  Call2(Engine_ActorSetAttachedEffect, 8, 0x102);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(Func_02003298, 8, 0, 20);
  Engine_ActorShowEmote(8, 0x101, 20);
  Call2(Engine_EventShowMessage, 8, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0x5000, 40);
  Call3(ObjectMotion_ArmCallback, 8, 0xb000, 40);
  Call3(ObjectMotion_ArmCallback, 8, 0x8000, 40);
  Engine_ActorShowEmote(8, 0x100, 20);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3(ObjectMotion_SetPositionAndReset, 8, 0x1a0, 0x2a8);
  Call2(Engine_EventShowMessage, 8, 0);
  Call3(ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x8000, 20);
  Call2(Engine_ActorSetAttachedEffect, gPartyState.currentOwner, 0x102);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Engine_EventShowMessage, 8, 0);
  ObjectMotion_SetAngleToward(8, gPartyState.currentOwner, 0);
  Call1(FieldEvent_WaitFrames, 20);
  ObjectMotion_SetAngleToward(gPartyState.currentOwner, 8, 0);
  Call2(Engine_EventShowMessage, 8, 0);
  Call2((void (*)())Engine_ActorSetAnimation, 8, 2);
  leader = Object_GetById(gPartyState.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(8);
  Call3(Func_02003228, 8, 0, 0);
  UiText_ShowPositionedMessageAndWait(message + 8, 1);
  Call1(GameFlag_SetBit, 0x863);
  Engine_EventEnd();
}

// A scene of actor 5 and the leader that ends by setting flag 0x864.
void Func_020002b4(void) {
  struct ObjectRuntime *actor;
  struct ObjectRuntime *leader;

  Engine_EventBegin();
  Func_02003310(0);
  Motion_SetVarCbAndRefresh(5, 1);
  FieldEvent_WaitFrames(40);
  Motion_SetVarCbAndRefresh(5, 2);
  FieldEvent_WaitFrames(20);
  Engine_EventSetMessage(MsgIdejimaAwakeningExclamation);
  Func_02003298(5, 0, 40);
  Value3(Engine_ActorShowEmote, 5, 0x101, 20);
  Engine_EventShowMessage(5, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 17, 46);
  actor = Object_GetById(5);
  Engine_ActorSetAnimation(5, 1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 1);
  Task_Wait(1);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 1);
  actor->velocityY = 0x40000;
  actor->motionFlags = 3;
  Task_Wait(40);
  Engine_ActorShowEmote(5, 0x100, 20);
  Engine_EventShowMessage(5, 0);
  Call3(ObjectMotion_ArmCallback, 5, 0x6000, 20);
  Call3(ObjectMotion_ArmCallback, 5, 0xa000, 20);
  Call3(ObjectMotion_ArmCallback, 5, 0x8000, 40);
  ObjectMotion_Launch(5, 4, 20);
  Engine_EventShowMessage(5, 0);
  ObjectMotion_SetAngleToward(5, gPartyState.currentOwner, 0);
  FieldEvent_WaitFrames(20);
  ObjectMotion_SetAngleToward(gPartyState.currentOwner, 5, 0);
  Engine_ActorShowEmote(5, 0x108, 0);
  Engine_EventShowMessage(5, 0);
  Motion_SetModeAndWaitAnimation(gPartyState.currentOwner, 3);
  FieldEvent_WaitFrames(40);
  Value3(Engine_ActorShowEmote, 5, 0x101, 0);
  Value2(UiText_OpenMessageAtObject, 5, 0);
  if (Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0) == 0) {
    Engine_ActorSetAnimation(5, 3);
    Engine_EventShowMessage(5, 0);
  } else {
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Engine_EventShowMessage(5, 0);
  }
  Call3(ObjectMotion_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Engine_ActorSetAnimation(5, 2);
  leader = Object_GetById(gPartyState.currentOwner);
  if (leader != 0)
    ObjectMotion_ResetAndSetPosition(5, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(5);
  Func_02003228(5, 0, 0);
  Event_PrepareObjectAndApplyValue(5, 1);
  GameFlag_SetBit(0x864);
  Engine_EventEnd();
}
