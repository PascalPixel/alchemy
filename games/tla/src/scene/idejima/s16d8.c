#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Import_EventStart(void);
void Import_EventEnd();
void Import_ActorPlaceAtActor();
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_WaitForAnimationChange();
void DriftScene_SpawnObject();
void Func_02003180();

extern const u8 gData_02003330[];
extern const u8 gData_0200336c[];

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
  Func_02003180();
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  east = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(184), PIXELS(168),
                                                 east);
  {
    struct Actor *ship = Actor_Get(8);

    ship->angle = FACING_WEST;
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
