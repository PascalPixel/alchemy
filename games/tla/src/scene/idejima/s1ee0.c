#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 gData_020033a8[];
extern const u8 gData_02003400[];
extern const u8 gData_0200346c[];
extern const u8 gData_020034e4[];
extern const u8 gData_0200350c[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_StartScreenTransition();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_0200161c(void);
void DriftScene_Prepare(void);
void Func_02003180();

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
  Actor_SetPosition(gMainRoutineWork.currentOwner,
                                         PIXELS(432), PIXELS(234));
  Actor_Get(gMainRoutineWork.currentOwner)->scaleX = 0xffff0000;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetPosition(10, PIXELS(392), PIXELS(172));
  System_Wait(1);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(214), 0);
  Func_02003180();
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
  actor->action = 0;
  Actor_SetScript(5, gData_0200346c);
  do {
    System_Wait(1);
  } while (actor->action == 0);
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
