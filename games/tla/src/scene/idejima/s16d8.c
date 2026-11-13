/// @file
/// Scene code of overlay 0x650, in ROM order, from s16d8's offset up to the
/// next object the overlay links, and the overlay's data.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "field_event.h"
#include "input.h"
#include "task.h"
#include "scene_ids.h"
#include "field.h"

void Import_ActorPlaceAtActor();
void DriftScene_SpawnObject();
void Map_CenterOnFollowedPoint();

extern u32 gData_02003330[];
extern u32 gData_0200336c[];

/// Idejima: the scene of actors 5, 8 and 10 that opens with
/// MsgIdejimaIsThisThePlace, ending in a screen transition and countdown 11.
void Func_020016d8(void) {
  /* FAKEMATCH(temporary): 0x4000 in a local, which the native keeps in r10
   * for the later arms; as the literal, 2 of 829 instructions differ in
   * tla-en. */
  s32 east;

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(252), -1, PIXELS(216), 0);
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  east = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(184), PIXELS(168), east);
  {
    struct Actor *ship = Actor_Get(8);

    ship->facing = FACING_WEST;
  }
  Actor_SetPosition(10, PIXELS(186), PIXELS(164));
  Actor_SetSpriteColor(Actor_Get(10), 15);
  Actor_SetSingleChildFlags(Actor_Get(10), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  FieldCamera_SetSpeed(0x9999, 0x1333);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_WalkToAndWait(5, 204, 196);
  Actor_WalkToAndWait(5, 208, 218);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Import_ActorPlaceAtActor(8, 5);
  System_Wait(1);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 186, 216);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 60);
  Actor_FaceDirection(8, FACING_WEST, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(8, FACING_EAST, 0);
  FieldEvent_SetMessageId(MsgIdejimaIsThisThePlace);
  Event_OpenMessage(0x1008, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_AskYesNoWithActorAnimation(5, 0);
  Actor_WaitForAnimationChange(5);
  Event_ShowMessage(0x1008, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(8, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(0x1008, 0, 40);
  Actor_ShowEmote(8, 0x106, 40);
  Event_ShowMessage(0x1008, 0);
  Actor_SetAnimationAndWait(5, 3);
  FieldActor_Jump(8, 4, 0);
  Event_ShowMessage(0x1008, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_SetAnimationAndWait(8, 3);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetScript(8, gData_02003330);
  FieldEvent_Wait(40);
  Actor_RunScriptAndWait(5, gData_0200336c);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_ShowEmote(8, 0x100, 20);
  Actor_FaceDirection(8, FACING_WEST, 40);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  Actor_WalkToAndWait(8, 258, 172);
  Event_ShowMessage(0x2008, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_SetSpeed(5, 0x6666, 0x3333);
  Actor_WalkToAndWait(5, 242, 172);
  Actor_WalkToAndWait(8, 248, 156);
  Actor_WalkToAndWait(8, 226, 164);
  Actor_WalkToAndWait(8, 228, 180);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 40);
  Event_ShowMessage(8, 0);
  Actor_WalkToAndWait(8, 226, 164);
  Actor_WalkToAndWait(8, 248, 156);
  Actor_WalkToAndWait(8, 258, 172);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Actor_ShowEmote(8, 0x105, 20);
  Event_ShowMessage(0x2008, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 256, 198);
  Actor_SetPosition(8, 0, 0);
  Actor_FaceDirection(5, east, 20);
  Actor_ShowEmote(5, 0x105, 40);
  FieldEvent_ShowMessageThenWait(0x8005, 0, 20);
  Actor_WalkToAndWait(5, 224, 182);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 40);
  Actor_ShowEmote(5, 0x105, 80);
  Actor_WalkToAndWait(5, 242, 172);
  Actor_FaceDirection(5, east, 40);
  Actor_ShowEmote(5, 0x105, 120);
  Actor_ShowEmote(5, 0x107, 40);
  FieldActor_Jump(5, 2, 10);
  FieldActor_Jump(5, 4, 20);
  FieldEvent_ShowMessageThenWait(0x8005, 0, 40);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(PIXELS(262), -1, PIXELS(180), 1);
  Actor_SetPosition(8, PIXELS(256), PIXELS(198));
  System_Wait(1);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 258, 172);
  FieldEvent_Wait(40);
  Actor_ShowEmote(8, 0x105, 80);
  Event_ShowMessage(0x8005, 0);
  Actor_SetAnimationAndWait(8, 4);
  Event_ShowMessage(0xa008, 0);
  Actor_ShowEmote(5, 0x101, 0);
  FieldEvent_ShowMessageThenWait(0x8005, 0, 20);
  Event_ShowMessage(0xa008, 0);
  DriftScene_SpawnObject(0xee0000, 0x300000, 0xac0000, 0xe000);
  FieldEvent_Wait(10);
  DriftScene_SpawnObject(0xf60000, 0x300000, 0xac0000, 0x2000);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 40);
  FieldEvent_ShowMessageThenWait(0x6008, 0, 20);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_WalkToAndWait(5, 248, 148);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(8, 0x101, 20);
  Event_ShowMessage(0xa008, 0);
  Actor_FaceDirection(5, east, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 20);
  Actor_SetAnimationAndWait(8, 4);
  Event_ShowMessage(0xa008, 0);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(8, 258, 170);
  FieldEvent_Wait(20);
  Actor_ShowEmote(8, 0x106, 40);
  Event_ShowMessage(0xa008, 0);
  Actor_ShowEmote(5, 0x101, 0);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(8, 3);
  Event_ShowMessage(0xa008, 0);
  Actor_SetAnimation(5, 3);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(0xa008, 0);
  Event_ShowMessage(0x900a, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_SetSingleChildFlags(Actor_Get(10), 1);
  Actor_SetSpriteColor(Actor_Get(10), 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, FACING_EAST, 40);
  Actor_SetAnimation(10, 3);
  Event_ShowMessage(0x900a, 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkToAndWait(10, 216, 164);
  Actor_WalkToAndWait(10, 234, 160);
  FieldEvent_Wait(40);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessage(0x800a, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(5, 3);
  Actor_ShowEmote(8, 0x101, 40);
  Event_ShowMessage(0xa008, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_ShowMessage(0x800a, 0);
  Event_ShowMessage(0xa008, 0);
  Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 40);
  Event_ShowMessage(0x800a, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

extern u32 gData_020033a8[];
extern u32 gData_02003400[];
extern u32 gData_0200346c[];
extern u32 gData_020034e4[];
extern u32 gData_0200350c[];

void Func_0200161c(void);
void DriftScene_Prepare(void);

void Func_02001ee0(void) {
  struct Actor *actor = Actor_Get(5);

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Func_0200161c();
  DriftScene_Prepare();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetPosition(6, PIXELS(444), PIXELS(212));
  Actor_SetAnimation(6, 19);
  Actor_SetSingleChildFlags(Actor_Get(6), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(432), PIXELS(234));
  Actor_Get(gMainRoutineWork.currentOwner)->scaleX = 0xffff0000;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetPosition(10, PIXELS(392), PIXELS(172));
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(214), 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetPosition(5, PIXELS(208), PIXELS(218));
  Actor_SetAnimation(5, 19);
  Actor_SetSingleChildFlags(Actor_Get(5), 0);
  Actor_SetPosition(8, PIXELS(176), PIXELS(204));
  Actor_SetAnimation(8, 5);
  Actor_SetSingleChildFlags(Actor_Get(8), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(120);
  FieldEvent_SetMessageId((s32)MsgIdejimaHowLongWillWeDrift);
  FieldEvent_ShowMessageThenWait(5, 0, 40);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(5, 263, 0);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(8, 261, 80);
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(8, 261, 40);
  Actor_ShowEmote(5, 265, 80);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(5, 258, 0);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(8, 258);
  FieldEvent_Wait(40);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(5, 261, 40);
  FieldEvent_ShowMessageThenWait(8, 0, 40);
  Actor_ShowEmote(5, 265, 20);
  FieldEvent_ShowMessageThenWait(5, 0, 20);
  FieldEvent_ShowMessageThenWait(8, 0, 40);
  Actor_ShowEmote(5, 261, 20);
  Event_ShowMessage(5, 0);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkToAndWait(10, 304, 172);
  Actor_WalkToAndWait(10, 224, 172);
  Actor_WalkToAndWait(10, 208, 192);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(8, 257, 0);
  Actor_ShowEmote(5, 257, 80);
  Event_ShowMessage(10, 0);
  Actor_SetScript(10, gData_020033a8);
  FieldEvent_Wait(40);
  Actor_SetAnimation(8, 1);
  Actor_SetSingleChildFlags(Actor_Get(8), 1);
  FieldActor_Jump(8, 4, 20);
  Actor_SetAnimation(5, 1);
  Actor_SetSingleChildFlags(Actor_Get(5), 1);
  FieldActor_Jump(5, 4, 20);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 40);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetScript(8, gData_02003400);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(368), -1, PIXELS(210), 1);
  actor->timer = 0;
  Actor_SetScript(5, gData_0200346c);
  do {
    System_Wait(1);
  } while (actor->timer == 0);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(8, 258);
  Actor_SetAttachedEffect(5, 258);
  FieldEvent_ShowTwoMessagesAndWait(5, 7, 13, 2, 12, 8, 9, 2, 4, 1, 0);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  Actor_SetSpeed(5, 0x19999, 0xcccc);
  Actor_SetScript(8, gData_020034e4);
  Actor_RunScriptAndWait(5, gData_0200350c);
  FieldEvent_Wait(40);
  Event_ShowMessage(0x5008, 0);
  FieldActor_Jump(5, 4, 10);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_ShowMessageThenWait(0x8008, 0, 40);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1b4) = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(12);
}

extern u32 gData_02003534[];
extern u32 gData_02003578[];
extern u32 gData_020035bc[];
extern u32 gData_02003600[];

void Func_020032e0(void);

void Func_02002304(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 a3000, aD000, a4000, a8000;

  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  DriftScene_Prepare();
  a3000 = 0x3000;
  Actor_SetPositionAndFacing(8, PIXELS(314), PIXELS(200), a3000);
  aD000 = 0xd000;
  Actor_SetPositionAndFacing(10, PIXELS(328), PIXELS(240), aD000);
  a4000 = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(340), PIXELS(200), a4000);
  Actor_SetPositionAndFacing(6, PIXELS(342), PIXELS(230), FACING_EAST);
  Actor_SetAnimation(6, 19);
  Actor_SetSingleChildFlags(Actor_Get(6), 0);
  a8000 = 0x8000;
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(312),
                             PIXELS(230), a8000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaThoughtTheyWereInsideLighthouse);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(0x200a, 0);
  Actor_SetAnimation(5, 4);
  Event_ShowMessage(5, 0);
  Actor_SetAnimationAndWait(8, 3);
  Actor_RunRepeatedMotion(6, 2);
  Actor_FaceDirection(10, aD000, 0);
  Actor_ShowEmote(10, 0x100, 80);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_ShowTwoMessagesAndWait(8, 9, 1, 7, 6, 10, 12, 16, 18, 11, 0);
  Actor_ShowEmote(8, 0x101, 40);
  Actor_RunRepeatedMotion(6, 2);
  Actor_SetAttachedEffect(5, 0x102);
  FieldEvent_ShowMessageThenWait(5, 0, 20);
  Actor_RunRepeatedMotion(6, 1);
  FieldEvent_Wait(40);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Actor_ShowEmote(6, 0x101, 40);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(6, 18);
  FieldEvent_Wait(40);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(8, 3);
  FieldEvent_ShowMessageThenWait(8, 0, 40);
  FieldEvent_ShowMessageThenWait(6, 0, 40);
  Actor_Get(6)->actionFlags &= 254;
  Actor_WalkTo(6, 340, 226);
  FieldActor_Jump(6, 4, 0);
  Actor_SetAnimation(6, 1);
  Actor_SetSingleChildFlags(Actor_Get(6), 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(5, 0x102);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_Get(6)->actionFlags |= 1;
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldActor_RefreshSelectorById(6);
  Actor_SetAnimationAndWait(6, 3);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(8, 6);
  FieldActor_WatchAndSetScript(5, 6);
  FieldActor_WatchAndSetScript(10, 6);
  Actor_SetSpeed(6, 0x6666, 0x3333);
  Actor_WalkToAndWait(6, 358, 222);
  FieldEvent_Wait(40);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 80);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, a8000, 0);
  FieldEvent_ShowMessageThenWait(6, 0, 20);
  Event_ShowMessage(0x100a, 0);
  Actor_FaceDirection(6, FACING_NORTHEAST, 40);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(6, 0x101, 40);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(8, 4);
  Event_ShowMessage(8, 0);
  FieldActor_Jump(5, 4, 0);
  Event_ShowMessage(0x1005, 0);
  Actor_FaceDirection(6, a8000, 20);
  Actor_SetAnimationAndWait(10, 3);
  Event_ShowMessage(0x100a, 0);
  Actor_SetSpeed(6, 0x19999, 0xcccc);
  Actor_WalkToAndWait(6, 348, 222);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Event_ShowMessage(0x4006, 0);
  FieldActor_Stop(8);
  FieldActor_Stop(5);
  FieldActor_Stop(10);
  Actor_SetSpeed(8, 0x6666, 0x3333);
  Actor_WalkToAndWait(8, 320, 200);
  FieldEvent_ShowMessageThenWait(8, 0, 40);
  Actor_FaceDirection(6, a8000, 40);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(10, 0x100, 0);
  Event_ShowMessage(0x100a, 0);
  Actor_SetAnimationAndWait(6, 4);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Event_ShowMessage(0x4006, 0);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(5, a8000, 0);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(0x100a, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  FieldActor_RefreshSelectorById(6);
  Actor_SetAnimationAndWait(6, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 4);
  Event_ShowMessage(0x100a, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST, 0);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldActor_RefreshSelectorById(6);
  Actor_SetAnimation(6, 4);
  FieldEvent_ShowMessageThenWait(6, 0, 20);
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(5, a4000, 0);
  FieldEvent_ShowMessageThenWait(0x1005, 0, 20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x105, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(6);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x105, 0);
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  Actor_SetAttachedEffect(8, 0x102);
  FieldEvent_ShowMessageThenWait(8, 0, 20);
  FieldActor_StartRepeatedMotion(gMainRoutineWork.currentOwner, 3);
  Actor_ShowEmote(10, 0x100, 20);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 20);
  Event_ShowMessage(10, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(6, a8000, 20);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(5, 0x109, 40);
  Event_ShowMessage(5, 0);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 254;
  Actor_WalkTo(gMainRoutineWork.currentOwner, 314, 226);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  leader->scaleX = 0x10000;
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(5, 0x102);
  Event_ShowMessage(5, 0);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 80);
  Event_ShowMessage(0x2008, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 20);
  Actor_FaceDirection(10, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(8, a3000, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(10, 3);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(5, a8000, 40);
  Actor_ShowEmote(5, 0x109, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(8, a3000, 0);
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(5, a8000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(6, a8000, 20);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Actor_SetAnimation(8, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetSpeed(10, 0x19999, 0xcccc);
  Actor_WalkToAndWait(10, 368, 256);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  FieldActor_RefreshSelectorById(6);
  Event_ShowMessage(0x5006, 0);
  Actor_FaceDirection(8, a3000, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_WalkToAndWait(10, 400, 264);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(10, 0x108, 0);
  Event_ShowMessage(0x100a, 0);
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(PIXELS(396), -1, PIXELS(234), 1);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  leader->timer = 0;
  Actor_SetScript(8, gData_020035bc);
  Actor_SetScript(5, gData_02003534);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02003600);
  Actor_SetScript(6, gData_02003578);
  do
    System_Wait(1);
  while (leader->timer == 0);
  Func_020032e0();
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(0x1005, 0);
  Event_ShowMessage(0x1006, 0);
  Event_ShowMessage(8, 0);
  FieldEvent_SetExitRequest(13);
}

void FelixWake_StartStep(void) {}

void FelixWake_EndStep(void) {}

void FelixWake_WaitForButton(void) {
  s32 frames = 180;

  FelixWake_StartStep();
wait:
  if (--frames != -1) {
    System_Wait(1);
    if (gInput.pressed == 0) {
      goto wait;
    }
  }
  FelixWake_EndStep();
}

/// The leader wakes, step by step at each button press, and is asked about
/// injuries; the scene ends by clearing flag 0x12f and setting flag 0x844.
void Func_02002c64(void) {
  s32 answer;
  s32 leader;

  FelixWake_StartStep();
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(384), -1, PIXELS(728), 0);
  System_Wait(1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  System_Wait(1);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(384), PIXELS(728));
  Actor_SetPosition(11, PIXELS(384), PIXELS(702));
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  System_Wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  System_Wait(1);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaCheckForInjuries);
  Event_OpenMessage(11, 0);
  System_Wait(1);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  leader = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 45);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 46);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 47);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 48);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  } else {
    Actor_WaitForAnimationChange(leader);
    *(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4) += 2;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 384, 746);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  }
  Actor_SetPosition(11, 0, 0);
  Event_ClearFlag(0x12f);
  Event_SetFlag(0x844);
  Import_EventEnd();
  FelixWake_EndStep();
}

void Func_0200153c(void);
void Func_02001558(void);
void Func_020015a0(void);
void Func_02002fd4(void);
void Func_020031a8(s32 a);

/// The scene's start entry: the two scene callbacks, then by entrance the
/// scene that runs (2 and 4 also set the saved scene and entrance).
s32 Scene_Initialize(void) {
  s32 entrance;

  Func_0200153c();
  Task_AddFrame(Func_02001558, TASK_PASS_FRAME);
  Task_AddFrame(Func_020015a0, TASK_PASS_VBLANK);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    entrance = party->entrance;
  }
  switch (entrance) {
  case 2:
    if (Event_GetFlag(0x109) == 0)
      Func_020031a8(1);
  case 4: {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->savedScene = (s32)&SceneId_DailaInteriors2;
    party->savedEntrance = 1;
  }
    Func_02002fd4();
    break;
  case 11:
    Func_020016d8();
    break;
  case 12:
    Func_02001ee0();
    break;
  case 13:
    Func_02002304();
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02002f88(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(Actor_Get(id), 0);
  object->facing = FACING_SOUTH;
  object->motionFlags = 0;
  object->x += 0xc0000;
  object->y = PIXELS(16);
  object->collisionFlags = 0;
  ((struct ActorSprite *)object->sprite)->rotation = 0xc000;
}

void Func_02002fd4(void) {
  s32 flag;

  if (!Event_GetFlag(0x863)) {
    struct Actor *actor;

    Actor_SetPosition(8, PIXELS(416), PIXELS(680));
    MapGrid_CopyCellAttributes(38, 32, 2, 1, 25, 42);
    actor = Actor_Get(8);
    Actor_SetSingleChildFlags(Actor_Get(8), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(8, 5);
  }
  if (!Event_GetFlag(0x864)) {
    struct Actor *actor;

    Actor_SetPosition(5, PIXELS(288), PIXELS(744));
    MapGrid_CopyCellAttributes(38, 33, 2, 1, 17, 46);
    actor = Actor_Get(5);
    Actor_SetSingleChildFlags(Actor_Get(5), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(5, 18);
  }
  flag = Event_GetFlag(0x865);
  if (flag == 0) {
    struct Actor *actor;

    actor = Actor_Get(6);
    Actor_SetPosition(6, PIXELS(240), PIXELS(648));
    MapGrid_CopyCellAttributes(38, 34, 2, 1, 14, 40);
    Actor_SetAnimation(6, 19);
    Actor_SetSingleChildFlags(Actor_Get(6), 0);
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
  }
  MapGrid_CopyCellsTo(72, 42, 75, 40, 1, 1);
  System_Wait(1);
  if (!Event_GetFlag(0x844))
    Func_02002c64();
}

#include "objcmd.h"

void Func_020000f0(void);
void Func_020002b4(void);
void Func_0200049c(void);
void Func_02000640(void);
void Func_020006a8(void);
void Idejima_RunAlexDeparture(void);

// clang-format off
u32 gData_02003330[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(164)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200336c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(182)),
    SC_SYNC,
    SC_MOVE(PIXELS(242), 0, PIXELS(164)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_020033a8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(224), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(392), 0, PIXELS(172)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003400[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(224), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(304), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(336), 0, PIXELS(192)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_0200346c[] = {
    39, 2, 4, 0xd80000, 0, 0xac0000,
    1, 4, 0x1280000, 0, 0xac0000, 1,
    4, 0x13c0000, 0, 0xd00000, 1, 39,
    1, 0, 1, 22, 3, 0,
    0, 1, 22, 35, 1, 17,
};

u32 gData_020034e4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(420), 0, PIXELS(200)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200350c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(408), 0, PIXELS(232)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02003534[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(200)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003578[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(408), 0, PIXELS(228)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_020035bc[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(400), 0, PIXELS(214)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003600[] = {
    39, 2, 4, 0x19c0000, 0, 0xf80000,
    1, 39, 1, 0, 1, 22,
    3, 0, 0, 1, 22, 35,
    1, 17,
};

s32 gData_02003650[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(4)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003734[] = {
    SC_POSITION(PIXELS(152), 0, PIXELS(756)),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(146), 0, PIXELS(736)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003790[] = {
    39, 2, 4, 0x980000, 0, 0x2f40000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gIdejima_ActorDepartureScript[] = {
    39, 2, 4, 0x680000, 0, 0x2d80000,
    1, 39, 1, 3, 0, 1,
    17,
};

struct SceneEntrance gIdejimaEntrances[] = {
    {0, -1, 172, 0, 149, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

u32 gIdejimaExits[] = {
    9, 0x303009, 0x401002, 0xb40002, 0xc03000, 0xd41002,
    511,
};

struct ActorPlacement gIdejimaPlacements[] = {
    {56, -1, 1, PIXELS(0), PIXELS(0), PIXELS(0), 0x4000, 2, 0},
    {5, -1, 1, PIXELS(0), PIXELS(0), PIXELS(0), 0x4000, 2, 0},
    {6, -1, 1, PIXELS(0), PIXELS(0), PIXELS(0), 0, 2, 0},
    {0x15a, -1, 7, PIXELS(256), PIXELS(0), PIXELS(200), 0x8000, 2, 0},
    {57, -1, 1, PIXELS(0), PIXELS(0), PIXELS(0), 0x4000, 2, 0},
    {0x1e9, -1, 7, PIXELS(0), PIXELS(0), PIXELS(0), 0x4000, 2, 0},
    {-1, 0, 0, PIXELS(0), PIXELS(0), PIXELS(0), 0, 0, 0},
};
// clang-format on

struct Trigger gIdejimaEvents[] = {
    {TRIGGERS_END},
};

struct Trigger gIdejimaEventsEntrance1[] = {
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000640)},
    {TRIGGERS_END},
};

struct Trigger gIdejimaEventsWake[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXAMINE, 10, 0x863, EVENT(Func_020000f0)},
    {TRIGGER_EXAMINE, 11, 0x864, EVENT(Func_020002b4)},
    {TRIGGER_EXAMINE, 12, 0x865, EVENT(Func_0200049c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT(Func_020000f0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x405, CONDITION_ALWAYS, EVENT(Func_020002b4)},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Func_020006a8)},
    {TRIGGER_STEP, 8, 0x867, EVENT(Idejima_RunAlexDeparture)},
    {TRIGGERS_END},
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
