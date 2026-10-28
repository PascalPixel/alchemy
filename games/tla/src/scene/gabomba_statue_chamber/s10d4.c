/// @file
/// The scene after the statue is lifted in the Gabomba Statue's chamber.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "call.h"
#include "field_sprite.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern const u8 MsgTheBlackOrb[];
extern const u8 gData_02001f18[];
s32 Actor_GetById();
void Func_02001da0(void);
void Func_02001ee8();
void Camera_MoveWithinMapBounds();
void Actor_SetPositionAndResetMotion();
void wait();
void Func_02001e18();
s32 HeapSlot_AllocFar();
void Func_02001d90();
void VramCache_Load();
void HeapSlot_Release();
void Func_02001010(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait();
void Actor_Jump();
void Func_02001e48();
void Func_02001e60();
void Func_02001e90();
void Func_02001e70();
s32 Func_02001e88();
void Func_02001f10();
s32 Func_02001eb8();
s32 Func_02001ec8();
void Actor_SetModeById();
void Actor_WatchAndSetScript();
void Func_02001e98();
void Actor_SetSpeedParameters();
void Actor_SetPositionAndCommit();
void Actor_Stop();
void Func_02001e10();
void Func_02001ef0(void);
void Actor_ArmCallback();
void Actor_SetPositionAndReset();
void Motion_SetModeAndWaitAnimation();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Actor_SetMovementTypeById();
void Event_FinishScreenTransition(void);
void GameFlag_SetBit();
void Func_02001eb0();

/// Places the party around the statue object (its sprite reloaded with item
/// 242's picture), raises and moves the statue while the camera follows, has
/// actor 12 ask a yes-or-no question (both answers count up the event work's
/// halfword 0x1c4), sends actors 5 and 12 off with their action scripts and
/// sets flag 265.
void Func_020010d4(void) {
  struct Actor *statue = (struct Actor *)Actor_GetById(13);
  struct FieldSprite *sprite;
  u8 *block;
  s32 turnA, turnB;
  s32 owner;

  Func_02001da0();
  Call1(Func_02001ee8, 0);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call4(Actor_SetPositionAndResetMotion, Value1(Actor_GetById, 13), 0x1880000,
        0, 0x10c0000);
  Call1(wait, 1);
  /* FAKEMATCH(temporary): the leader read before the first turn is built; as
   * gAdventure.currentOwner in the call, 34 of 640 instructions differ in
   * tla-en. */
  owner = gAdventure.currentOwner;
  Call4(Func_02001e18, owner, 0x1680000, 0xb80000, turnA = 0xe000);
  Call4(Func_02001e18, 7, 0x1780000, 0xb80000, turnA);
  Call4(Func_02001e18, 11, 0x1680000, 0xa80000, turnA);
  Call4(Func_02001e18, 6, 0x1a70000, 0xa80000, turnB = 0xa000);
  Call4(Func_02001e18, 5, 0x1980000, 0xb80000, turnB);
  if (statue != 0) {
    sprite = statue->animation;
    sprite->nextFrame = 0;
    sprite->unk1b[0] = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    ((u8 *)statue)[0x5c] = 1;
    block = (u8 *)Value2(HeapSlot_AllocFar, 68, 0x608);
    Call1(Func_02001d90, 242);
    Call3(VramCache_Load, sprite->vramBlock, 0x80, (s32)(block + 0x400));
    Call1(HeapSlot_Release, 68);
  }
  Func_02001010();
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_Jump, 7, 4, 10);
  Call1(Func_02001e48, MsgTheBlackOrb);
  Call2(Func_02001e60, 7, 0);
  Call2(Func_02001e90, 6, 258);
  Call2(Func_02001e70, 6, 0x8000);
  Call2(Func_02001e60, 0x4006, 0);
  Value3(Func_02001e88, 5, 265, 20);
  Call2(Func_02001e60, 5, 0);
  Call4(Actor_SetPositionAndResetMotion, Value1(Actor_GetById, 13), 0x1880000,
        0, 0x1540000);
  Call1(Func_02001f10, 78);
  Value2(Func_02001eb8, 0x10002, 0);
  Value1(Func_02001ec8, 40);
  Call1(wait, 40);
  Call2(Actor_SetModeById, 9, 2);
  Call2(Actor_SetModeById, 10, 2);
  Call2(Actor_WatchAndSetScript, gAdventure.currentOwner, 13);
  Call2(Actor_WatchAndSetScript, 5, 13);
  Call2(Actor_WatchAndSetScript, 6, 13);
  Call2(Actor_WatchAndSetScript, 7, 13);
  Call2(Actor_WatchAndSetScript, 11, 13);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02001e98, 0xcccc, 0x1999);
  Call4(Camera_MoveWithinMapBounds, 0x1880000, -1, 0xf60000, 1);
  Call3(Actor_SetSpeedParameters, 13, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndCommit, 13, 392, 246);
  Call2(Actor_SetModeById, 9, 1);
  Call2(Actor_SetModeById, 10, 1);
  statue->velocityY = 0x30000;
  statue->gravity = 0x8000;
  Call3(Actor_SetSpeedParameters, 13, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndCommit, 13, 392, 234);
  Call1(FieldEvent_Wait, 10);
  Call4(Camera_MoveWithinMapBounds, 0x1880000, -1, 0xa80000, 1);
  Call3(Actor_SetSpeedParameters, 13, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndCommit, 13, 392, 0xa0);
  Call1(Actor_Stop, gAdventure.currentOwner);
  Call1(Actor_Stop, 5);
  Call1(Actor_Stop, 6);
  Call1(Actor_Stop, 7);
  Call1(Actor_Stop, 11);
  Call1(wait, 1);
  Call3(Func_02001e10, 13, 0, 0);
  Call1(FieldEvent_Wait, 20);
  Value2(Func_02001eb8, 0x10000, 0);
  Value1(Func_02001ec8, 40);
  Call1(wait, 40);
  Func_02001ef0();
  Value3(Func_02001e88, 11, 0x100, 40);
  Call2(Func_02001e60, 0x400b, 0);
  Value3(Func_02001e88, 6, 261, 20);
  Call2(Func_02001e60, 0x4006, 0);
  Call2(Func_02001e70, 5, turnA);
  Call2(Func_02001e60, 0x1005, 0);
  Call3(Actor_ArmCallback, 5, 0xa000, 0);
  Call3(Actor_SetSpeedParameters, 6, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndReset, 6, 0x190, 172);
  Call2(Actor_SetModeById, 6, 9);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02001e60, 0x4006, 0);
  Call2(Actor_SetModeById, 7, 4);
  Call2(Func_02001e60, 7, 0);
  Call2(Func_02001e60, 0x4006, 0);
  Value3(Func_02001e88, 11, 0x100, 0);
  Call2(Func_02001e60, 0x400b, 0);
  Call2(Func_02001e70, 11, 0x3000);
  Call2(Func_02001e60, 0x400b, 0);
  Call2(Motion_SetModeAndWaitAnimation, gAdventure.currentOwner, 3);
  Call1(Func_02001f10, 0);
  {
    /* FAKEMATCH(temporary): the turn built before the other arguments; as a
     * plain 0xd000 argument, 2 of 640 instructions differ in tla-en. */
    s32 turn = 0xd000;
    Call4(Func_02001e18, 12, 0x1900000, 0x1400000, turn);
  }
  Call1(wait, 1);
  Call2(Func_02001e60, 12, 0);
  Call2(Actor_SetModeById, 6, 1);
  Call1(wait, 1);
  Call3(Actor_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call3(Actor_ArmCallback, 6, 0x4000, 0);
  Call3(Actor_ArmCallback, 7, 0x4000, 0);
  Call3(Actor_ArmCallback, 11, 0x3000, 0);
  Call2(Func_02001e98, 0x19999, 0x3333);
  Call4(Camera_MoveWithinMapBounds, 0x1880000, -1, 0xc80000, 1);
  Call3(Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call3(Actor_SetPositionAndReset, 12, 404, 220);
  Call3(Actor_Jump, 12, 4, 10);
  Value2(FieldMessage_ShowAtActor, 12, 0);
  if (Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0) ==
      0) {
    Call2(Func_02001e90, 12, 258);
    Call1(FieldEvent_Wait, 20);
    Call2(Func_02001e60, 12, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Value3(Func_02001e88, 12, 259, 20);
    Call2(Func_02001e60, 12, 0);
  }
  Value3(Func_02001e88, 7, 263, 20);
  Call2(Func_02001e60, 7, 0);
  Value3(Func_02001e88, 12, 0x100, 40);
  Call2(Func_02001e60, 12, 0);
  Value3(Func_02001e88, 6, 263, 20);
  Call2(Func_02001e60, 6, 0);
  Call2(Func_02001e90, 12, 258);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02001e60, 12, 0);
  Call2(Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2(Actor_SetModeById, 5, 3);
  Call2(Actor_SetModeById, 6, 3);
  Call2(Actor_SetModeById, 7, 3);
  Call2(Motion_SetModeAndWaitAnimation, 11, 3);
  Call2(Actor_SetModeById, 12, 4);
  Call2(Func_02001e60, 12, 0);
  Value3(Func_02001e88, 5, 261, 20);
  Call3(Actor_SetSpeedParameters, 5, 0xcccc, 0x6666);
  Call3(Actor_SetPositionAndReset, 5, 420, 228);
  Call2(Func_02001e70, 5, 0xa000);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 5, 412, 222);
  Call3(Actor_SetSpeedParameters, 5, 0x10000, 0x8000);
  Call3(Actor_SetSpeedParameters, 12, 0x10000, 0x8000);
  Call2(Actor_SetMovementTypeById, 5, gData_02001f18);
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_SetMovementTypeById, 12, gData_02001f18);
  Call1(FieldEvent_Wait, 60);
  *(s32 *)(EVENT_WORK + 0x1ac) = 514;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(GameFlag_SetBit, 265);
  Call1(Func_02001eb0, 2);
}
