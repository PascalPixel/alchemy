#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "madra_interiors.h"

extern u8 gData_0200349c[];
extern u8 gData_02003418[];
extern u8 gData_020033c8[];
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Field_DispatchTypeHandler();
void Func_02001d50();
void Func_02001d58();
void Func_02001e68();
void Func_02001e90();
void Func_02001e98();
void Func_02001ea0();
void Func_02001ea8();
void Func_02001eb0();
void Actor_SetSingleChildFlags();
void Actor_WaitForAnimationChange();

s32 Scene_PrepareMap(void) { return 0; }

s32 Func_020007ac(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation -= 0x800;
  return 0;
}

s32 Func_020007c8(struct Actor *object) {
  struct FieldSprite *sprite;

  sprite = object->animation;
  if (object != NULL && sprite != NULL)
    sprite->rotation = 0;
  return 0;
}

void Func_020007dc(void) {
  s32 i;
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;
  struct Actor *object8;
  struct Actor *object9;
  struct Actor *object10;
  struct Actor *object11;
  struct Actor *object12;
  s32 result13;
  struct Actor *object14;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(8, 1);
  Actor_SetPosition(4, PIXELS(400), PIXELS(888));
  FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(848), 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors28);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_WalkToAndWait(4, 396, 869);
  FieldActor_LaunchFromLeader(11, 25, 19, FACING_NORTH);
  FieldActor_RefreshSelectorById(11);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(11, FACING_NORTHEAST, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(820), 1);
  Func_02001e68();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x107, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_WalkByAndWait(7, 0, -16);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x103, 0);
  object0 = Actor_Get(9);
  object0->actionFlags &= 254;
  for (i = 2; i >= 0; i--) {
    Actor_WalkByAndWait(9, 0, -8);
    Event_PlaySound(134);
    Actor_WalkByAndWait(9, 0, 8);
  }
  object1 = Actor_Get(9);
  object1->actionFlags |= 1;
  FieldEvent_Wait(30);
  Actor_ShowEmote(7, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(7, 0x107, 30);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_FaceActor(10, 9, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_FaceActor(9, 10, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_ShowEmote(9, 0x107, 20);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(7, 0, 16);
  Func_02001e90(24, 1);
  Func_02001e98(7, 8);
  Func_02001eb0();
  Actor_ShowEmote(9, 0x101, 30);
  Actor_FaceActor(9, 7, 0);
  Actor_FaceActor(10, 7, 0);
  object2 = Actor_Get(9);
  Actor_SetSingleChildFlags(object2, 0);
  Actor_SetScript(9, gData_0200349c);
  Field_DispatchTypeHandler(1);
  Func_02001ea0();
  Func_02001ea8();
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  FieldActor_Stop(9);
  object3 = Actor_Get(9);
  object3->scaleX = 0x10000;
  object4 = Actor_Get(9);
  object4->scaleY = 0x10000;
  FieldEvent_Wait(4);
  object5 = Actor_Get(9);
  object5->motionFlags = 2;
  object6 = Actor_Get(9);
  object6->actionFlags &= 254;
  Actor_SetDestinationOffset(9, -8, 0);
  Actor_SetScript(9, gData_02003418);
  FieldActor_RefreshSelectorById(9);
  Event_PlaySound(127);
  FieldEvent_Wait(60);
  object7 = Actor_Get(9);
  object7->actionFlags = (object7->actionFlags | 1);
  FieldActor_Jump(9, 6, 0);
  FieldEvent_Wait(3);
  Actor_SetSpriteRotationById(9, 0);
  object8 = Actor_Get(9);
  Actor_SetSingleChildFlags(object8, 1);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_ShowEmote(10, 0x100, 30);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  Actor_FaceActor(9, 7, 0);
  FieldEvent_Wait(20);
  object9 = Actor_Get(10);
  object9->actionFlags &= 254;
  object10 = Actor_Get(9);
  object10->actionFlags &= 254;
  Actor_WalkByAndWait(9, -16, 16);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldActor_Jump(9, 6, 20);
  FieldEvent_Wait(20);
  object11 = Actor_Get(9);
  object11->actionFlags = (object11->actionFlags | 1);
  Actor_SetScript(9, gData_020033c8);
  FieldActor_WatchAndSetScript(4, 9);
  FieldActor_WatchAndSetScript(11, 9);
  FieldEvent_ShowMessageThenWait(9, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 7, 0);
  FieldActor_WatchAndSetScript(4, 10);
  FieldActor_WatchAndSetScript(11, 10);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(7, 0x105, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x102, 30);
  Actor_WalkByAndWait(10, -16, 16);
  FieldEvent_ShowMessageThenWait(10, 0, 1);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 8);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  object12 = Actor_Get(10);
  object12->actionFlags |= 1;
  Actor_SetScript(10, gData_020033c8);
  FieldEvent_Wait(40);
  FieldActor_Stop(4);
  FieldActor_Stop(11);
  FieldActor_RefreshSelectorById(10);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetAnimation(8, 6);
  Actor_SetSpriteColorById(8, 0);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  Actor_WaitForAnimationChange(8);
  FieldCamera_FollowActor(4, 1);
  Func_02001e68();
  Actor_FaceEachOther(11, 4, 0);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  Event_OpenMessage(11, 0);
  result13 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result13 != 0)
    goto L_02008e40;
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_02008e64;
L_02008e40:
  Actor_SetAnimationAndWait(11, 4);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  FieldEvent_ShowMessageThenWait(11, 0, 10);
L_02008e64:
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(11, 2);
  object14 = Actor_Get(4);
  if (object14 == 0)
    goto L_02008eb2;
  Actor_SetDestination(11, *(s16 *)((u8 *)object14 + 10),
                            *(s16 *)((u8 *)object14 + 18));
L_02008eb2:
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0x911);
  Func_02001d58();
  return;
}

void Func_02000ed8(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors29);
  Actor_FaceDirection(11, FACING_SOUTH, 0);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Func_02001d58();
  return;
}

void Func_02000f0c(void) {
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_RunRepeatedMotion(7, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Func_02001d58();
  return;
}

void Func_02000fe0(void) {
  Func_02001e90(24, 1);
  Func_02001e98(7, 8);
  Func_02001eb0();
  Field_DispatchTypeHandler(1);
  Func_02001ea0();
  Func_02001ea8();
  return;
}
