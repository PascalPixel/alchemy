#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020014b8(void);
void Func_020014c0(void);
void Func_02001588(s32 scene, s32 entrance);

void Func_0200006c(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor02;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_020000b8(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor09;

  Func_020014b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(20);
    Actor_WatchAndSetScript(actor, gMainRoutineWork.currentOwner);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 760, 320);
    Actor_FaceDirection(14, 0x5000, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Event_Wait(10);
    Func_02001588((s32)&SceneId_WorldMap, 67);
  } else {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(actor, 0);
  }
  Func_020014c0();
}

void Func_02000150(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor14;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void Func_0200019c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8ba);
  Func_020014b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor18);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(18, 256, 60);
  Actor_SetAngleToward(18, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmote(18, 263, 20);
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 4);
  Event_ShowMessageThenWait(18, 0, 10);
  Func_020014c0();
}

void Func_02000238(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  GameFlag_SetBit(0x8bb);
  Func_020014b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor23);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(8, 256, 30);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 304, 152);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Actor_ArmCallback(8, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_ArmCallback(8, FACING_EAST, 0);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x4008, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  Event_ShowMessageThenWait(0x4008, 0, 10);
  Func_020014c0();
}

void Func_02000370(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)((u8 *)work + 0x166) != 0) {
    Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
  }
  GameFlag_SetBit(0x8bc);
  Func_020014b8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor24);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ArmCallback(9, 0xd000, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(9, 258, 40);
  Event_ShowMessageThenWait(9, 0, 20);
  Actor_ShowEmote(9, 264, 40);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(9, 0, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Event_Wait(30);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessageThenWait(9, 0, 10);
  }
  Func_020014c0();
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_FaceDirection(9, 0xd000, 0);
}
