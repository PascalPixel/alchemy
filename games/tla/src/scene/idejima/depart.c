#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "adventure.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "call.h"
#include "text/msg_ids.h"

void GameFlag_SetBit();
void FieldEvent_WaitFrames();
s32 Event_AskYesNoWithActorAnimation();
void Actor_SetSpeedParameters();
void Actor_SetMovementTypeById();
void Actor_RefreshSelectorById();
void Actor_Stop();
void Actor_SetMovementTypeAndWaitById();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetPositionAndReset();
void Actor_WaitForMoveById();
void Import_ActorPlaceAtActor();
void Motion_SetModeAndWaitAnimation();
void Actor_Jump();
s32 FieldMessage_ShowAtActor();
void Actor_ArmCallback();
s32 Import_ActorShowEmote();
void Import_ActorSetAttachedEffect();
void Import_CameraSetSpeed();
void Camera_MoveWithinMapBounds();
void Func_02003310();
void Actor_WatchAndSetScript();
void Func_02003320();

void Idejima_RunAlexDeparture(void) {
  const u8 *script;

  Import_EventStart();
  Func_02003310(0);
  Call2(Import_CameraSetSpeed, 0x9999, 0x1333);
  Call4(Camera_MoveWithinMapBounds, 0x6c0000, -1, 0x2d00000, 1);
  Call3(Actor_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
        0x6666);
  Call3(Actor_SetPositionAndReset, gAdventure.currentOwner, 86, 0x2c8);
  Import_ActorPlaceAtActor(8, gAdventure.currentOwner);
  Task_Wait(1);
  Call3(Actor_SetSpeedParameters, 8, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndReset, 8, 104, 0x2c8);
  Actor_ArmCallback(gAdventure.currentOwner, 0, 0);
  Call3(Actor_ArmCallback, 8, 0x8000, 20);
  Call1(Import_EventSetMessageId, (s32)MsgIdejimaWhereHasAlexGone);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Actor_ArmCallback(8, 0, 40);
  Call3(Actor_ArmCallback, 8, 0x5000, 20);
  Actor_ArmCallback(8, 0, 0);
  Call3(Actor_SetPositionAndReset, gAdventure.currentOwner, 104, 0x2d8);
  Actor_ArmCallback(gAdventure.currentOwner, 0, 20);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x2000, 20);
  Actor_ArmCallback(gAdventure.currentOwner, 0, 0);
  Import_EventShowMessageAndWait(8, 0);
  Call3(Actor_ArmCallback, 8, 0x3000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xc000, 20);
  Import_ActorSetAnimation(8, 3);
  Value2(Import_EventAskYesNoWithActorAnimation, 8, 0);
  Value3(Import_ActorShowEmote, 8, 0x106, 40);
  Import_EventShowMessageAndWait(8, 0);
  Call2(Import_ActorPlaceAtActor, 5, gAdventure.currentOwner);
  Import_ActorPlaceAtActor(6, gAdventure.currentOwner);
  Task_Wait(1);
  Call3(Actor_SetSpeedParameters, 6, 0xcccc, 0x6666);
  Call3(Actor_SetSpeedParameters, 5, 0xcccc, 0x6666);
  Call3(Actor_ResetAndSetPositionInMode2, 5, 86, 0x2c8);
  Call3(Actor_SetPositionAndReset, 6, 86, 0x2d8);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Actor_WaitForMoveById(5);
  Import_ActorSetAnimation(5, 1);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x8000, 0);
  Actor_ArmCallback(6, 0, 40);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xc000, 0);
  Import_ActorSetAnimation(8, 4);
  Value2(FieldMessage_ShowAtActor, 0x2008, 0);
  Actor_ArmCallback(6, 0, 0);
  Call3(Actor_ArmCallback, 5, 0x2000, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Motion_SetModeAndWaitAnimation(8, 4);
    Call2(Import_EventShowMessageAndWait, 0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  }
  Actor_ArmCallback(5, 0, 0);
  Import_EventShowMessageAndWait(5, 0);
  Value3(Import_ActorShowEmote, 6, 0x101, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 8, 0x5000, 0);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Call3(Import_ActorShowEmote, gAdventure.currentOwner, 0x101, 40);
  Call3(Actor_ArmCallback, 8, 0x3000, 0);
  Import_ActorSetAnimation(8, 4);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Value3(Import_ActorShowEmote, 5, 0x100, 20);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 8, 0x8000, 20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Actor_ArmCallback(6, 0, 20);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 5, 0x2000, 0);
  Value2(FieldMessage_ShowAtActor, 5, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, 8, 0) == 0) {
    Call3(Actor_ArmCallback, 8, 0x3000, 20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2(Import_EventShowMessageAndWait, 0x2008, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Value3(Import_ActorShowEmote, 8, 0x103, 0);
    Call3(Actor_ArmCallback, 8, 0x4000, 20);
    Call2(Import_EventShowMessageAndWait, 0x2008, 0);
    Func_02003320();
  }
  Value3(Import_ActorShowEmote, 5, 0x100, 0);
  Import_EventShowMessageAndWait(5, 0);
  Value3(Import_ActorShowEmote, 8, 0x100, 40);
  Call3(Actor_ArmCallback, 8, 0x5000, 0);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Value3(Import_ActorShowEmote, 6, 0x101, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 40);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 20);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 8, 0x3000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xc000, 20);
  Value2(FieldMessage_ShowAtActor, 0x2008, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Value3(Import_ActorShowEmote, 6, 0x103, 0);
    Actor_ArmCallback(6, 0, 20);
    Import_EventShowMessageAndWait(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Value3(Import_ActorShowEmote, 6, 0x108, 0);
    Actor_ArmCallback(6, 0, 20);
    Import_EventShowMessageAndWait(6, 0);
  }
  Value3(Import_ActorShowEmote, 8, 0x100, 0);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Import_EventShowMessageAndWait(6, 0);
  Value3(Import_ActorShowEmote, 5, 0x101, 40);
  Import_EventShowMessageAndWait(5, 0);
  Actor_WatchAndSetScript(gAdventure.currentOwner, 6);
  Actor_WatchAndSetScript(8, 6);
  Actor_WatchAndSetScript(5, 6);
  Call3(Actor_SetSpeedParameters, 6, 0x6666, 0x3333);
  Call3(Actor_SetPositionAndReset, 6, 86, 0x2ec);
  FieldEvent_WaitFrames(40);
  Value3(Import_ActorShowEmote, 6, 0x109, 80);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Actor_Stop(gAdventure.currentOwner);
  Actor_Stop(8);
  Actor_Stop(5);
  Motion_SetModeAndWaitAnimation(6, 3);
  FieldEvent_WaitFrames(40);
  Import_ActorSetAnimation(5, 4);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Actor_RefreshSelectorById(6);
  Import_ActorSetAnimation(6, 4);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 8, 0x3000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0xc000, 40);
  Call3(Actor_ArmCallback, 8, 0x5000, 0);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x6000, 20);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Import_EventShowMessageAndWait(6, 0);
  Import_ActorSetAnimation(8, 3);
  Import_ActorSetAnimation(5, 3);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 8, 0x3000, 0);
  Value2(Import_EventAskYesNoWithActorAnimation, 0x2008, 0);
  Call3(Actor_ArmCallback, 6, 0xa000, 40);
  Import_EventShowMessageAndWait(6, 0);
  Actor_ArmCallback(5, 0, 0);
  Call3(Actor_ArmCallback, 8, 0x8000, 20);
  Import_ActorSetAnimation(6, 4);
  Import_EventShowMessageAndWait(6, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call3(Actor_ArmCallback, 8, 0x5000, 0);
  Value3(Import_ActorShowEmote, 8, 0x105, 0);
  Value3(Import_ActorShowEmote, 5, 0x105, 40);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Import_EventShowMessageAndWait(6, 0);
  Call2(Import_ActorSetAttachedEffect, 8, 0x102);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Value3(Import_ActorShowEmote, 6, 0x108, 20);
  Call3(Actor_ArmCallback, 6, 0xe000, 20);
  Import_EventShowMessageAndWait(6, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Import_EventShowMessageAndWait(5, 0);
  Call3(Actor_ArmCallback, 5, 0x2000, 0);
  Value2(FieldMessage_ShowAtActor, 5, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Import_ActorSetAnimation(5, 3);
    Import_EventShowMessageAndWait(5, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Import_ActorSetAnimation(5, 4);
    Import_EventShowMessageAndWait(5, 0);
  }
  Value3(Import_ActorShowEmote, 8, 0x109, 40);
  Motion_SetModeAndWaitAnimation(6, 4);
  FieldEvent_WaitFrames(20);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Motion_SetModeAndWaitAnimation(6, 3);
  Call3(Actor_ArmCallback, 8, 0x8000, 20);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Actor_Jump(8, 4, 20);
  Call2(Import_EventShowMessageAndWait, 0x2008, 0);
  Call3(Actor_SetSpeedParameters, 6, 0xcccc, 0x6666);
  script = gIdejima_ActorDepartureScript;
  Actor_SetMovementTypeById(8, script);
  Call2(Actor_SetMovementTypeById, 5, script);
  Actor_SetMovementTypeAndWaitById(6, script);
  Call1(GameFlag_SetBit, 0x867);
  Import_EventEnd();
}

extern s16 gData_02003c14;
extern s32 gData_02003c10;

void Func_0200153c(void) {
  gData_02003c14 = 0;
  gData_02003c10 = 0;
}
