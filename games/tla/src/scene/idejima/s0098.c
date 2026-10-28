#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById(s32 id);
void Import_ActorSetAnimation(s32 actor, s32 animation);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Task_Wait(s32 frames);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02000098(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Import_ActorSetAnimation(id, 1);
  Actor_SetSingleChildFlags(Actor_GetById(id), 1);
  object->x -= 0xc0000;
  ((struct ActorSprite *)object->animation)->rotation = 0;
  Task_Wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(id), 1);
  object->velocityY = 0x40000;
  object->motionFlags = 3;
  Task_Wait(40);
}

void Import_EventStart();
void Import_EventEnd();
void Import_EventSetMessageId();
void Import_EventShowMessageAndWait();
void GameFlag_SetBit();
void FieldEvent_WaitFrames();
void Actor_SetSpeedParameters();
void Actor_SetPositionAndReset();
void Actor_WaitForMoveById();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_ArmCallback();
s32 Import_ActorShowEmote();
void Import_ActorSetAttachedEffect();
void Func_02003310();
void Func_02003190();
void Func_02003228();
void Func_02003298();
void Motion_SetVarCbAndRefresh();
void FieldMessage_Show();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Event_PrepareObjectAndApplyValue();
void Motion_SetModeAndWaitAnimation();
void Actor_Jump();

/// A scene of actor 8 and the leader that ends by setting flag 0x863.
void Func_020000f0(void) {
  s32 message;
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  Func_02003310(0);
  Call2(Motion_SetVarCbAndRefresh, 8, 1);
  message = (s32)MsgIdejimaGroan;
  Import_EventSetMessageId(message);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Call2(Motion_SetVarCbAndRefresh, 8, 2);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 25, 42);
  actor = Actor_GetById(8);
  Call2((void (*)())Import_ActorSetAnimation, 8, 1);
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  Task_Wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(8), 1);
  actor->velocityY = 0x40000;
  actor->motionFlags = 3;
  Task_Wait(40);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Actor_SetAngleToward(8, gAdventure.currentOwner, 0);
  Call2(Import_ActorSetAttachedEffect, 8, 0x102);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(Func_02003298, 8, 0, 20);
  Import_ActorShowEmote(8, 0x101, 20);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Call3(Actor_ArmCallback, 8, 0x5000, 40);
  Call3(Actor_ArmCallback, 8, 0xb000, 40);
  Call3(Actor_ArmCallback, 8, 0x8000, 40);
  Import_ActorShowEmote(8, 0x100, 20);
  Call3(Actor_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndReset, 8, 0x1a0, 0x2a8);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 20);
  Call2(Import_ActorSetAttachedEffect, gAdventure.currentOwner, 0x102);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Actor_SetAngleToward(8, gAdventure.currentOwner, 0);
  Call1(FieldEvent_WaitFrames, 20);
  Actor_SetAngleToward(gAdventure.currentOwner, 8, 0);
  Call2(Import_EventShowMessageAndWait, 8, 0);
  Call2((void (*)())Import_ActorSetAnimation, 8, 2);
  leader = Actor_GetById(gAdventure.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(8, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(8);
  Call3(Func_02003228, 8, 0, 0);
  FieldMessage_Show(message + 8, 1);
  Call1(GameFlag_SetBit, 0x863);
  Import_EventEnd();
}

/// A scene of actor 5 and the leader that ends by setting flag 0x864.
void Func_020002b4(void) {
  struct Actor *actor;
  struct Actor *leader;

  Import_EventStart();
  Func_02003310(0);
  Motion_SetVarCbAndRefresh(5, 1);
  FieldEvent_WaitFrames(40);
  Motion_SetVarCbAndRefresh(5, 2);
  FieldEvent_WaitFrames(20);
  Import_EventSetMessageId(MsgIdejimaAwakeningExclamation);
  Func_02003298(5, 0, 40);
  Value3(Import_ActorShowEmote, 5, 0x101, 20);
  Import_EventShowMessageAndWait(5, 0);
  Call6(Func_02003190, 38, 31, 2, 1, 17, 46);
  actor = Actor_GetById(5);
  Import_ActorSetAnimation(5, 1);
  Actor_SetSingleChildFlags(Actor_GetById(5), 1);
  Task_Wait(1);
  Actor_SetSingleChildFlags(Actor_GetById(5), 1);
  actor->velocityY = 0x40000;
  actor->motionFlags = 3;
  Task_Wait(40);
  Import_ActorShowEmote(5, 0x100, 20);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 5, 0x6000, 20);
  Call3(Actor_ArmCallback, 5, 0xa000, 20);
  Call3(Actor_ArmCallback, 5, 0x8000, 40);
  Actor_Jump(5, 4, 20);
  Import_EventShowMessageAndWait(5, 0);
  Actor_SetAngleToward(5, gAdventure.currentOwner, 0);
  FieldEvent_WaitFrames(20);
  Actor_SetAngleToward(gAdventure.currentOwner, 5, 0);
  Import_ActorShowEmote(5, 0x108, 0);
  Import_EventShowMessageAndWait(5, 0);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(40);
  Value3(Import_ActorShowEmote, 5, 0x101, 0);
  Value2(FieldMessage_ShowAtActor, 5, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Import_ActorSetAnimation(5, 3);
    Import_EventShowMessageAndWait(5, 0);
  } else {
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Import_EventShowMessageAndWait(5, 0);
  }
  Call3(Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Import_ActorSetAnimation(5, 2);
  leader = Actor_GetById(gAdventure.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(5, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Func_02003228(5, 0, 0);
  Event_PrepareObjectAndApplyValue(5, 1);
  GameFlag_SetBit(0x864);
  Import_EventEnd();
}
