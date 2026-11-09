#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

extern struct DoorStep gData_02005290[];

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02000394(s32, s32);
void Func_0200042c(s32, s32);
void Func_02000514(void);
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004bb4();
s32 Func_02004c44();
s32 Func_02004c4c();

void Func_020006ec(s32 mode) {
  s32 tile;

  tile = Func_02004bb4(gMainRoutineWork.currentOwner);
  if (tile == 16) {
    Func_02000394(mode, 16);
    if (Func_02004c44() != 0)
      Func_02004c4c();
    Func_0200042c(mode, 16);
  } else if (tile == 15) {
    Func_02000514();
  }
}

void Func_02000730(void) {
  Event_SetFlag(0x8a8);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgWhatSYour);
  Actor_ShowEmote(22, 0x101, 40);
  Actor_FaceActor(22, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(22, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(22, 0, 0);
  } else {
    FieldEvent_Wait(20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_OpenMessage(22, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
      FieldEvent_Wait(30);
      FieldEvent_ShowMessageThenWait(22, 0, 0);
    } else {
      FieldEvent_Wait(20);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
        FieldEvent_Wait(30);
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      } else {
        FieldEvent_Wait(20);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
        FieldEvent_ShowMessageThenWait(22, 0, 0);
      }
    }
  }
  Actor_FaceDirection(22, FACING_EAST, 0);
  Func_02004a9c();
}

void Func_02000838(void) {
  if (Event_GetFlag(0x8a7)) {
    if (Event_GetFlag(0x904) == 0) {
      Event_SetFlag(0x904);
      Func_02001cb0();
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x8a0) && Func_02004a84(151) == -1 &&
        Event_GetFlag(0x8ad) == 0) {
      Event_SetFlag(0x8ad);
      Func_020017ec();
    }
  } else if (Event_GetFlag(0x8a9) == 0) {
    Event_SetFlag(0x8a9);
    Func_0200112c();
  }
}

void Func_020008cc(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(158);
  MapGrid_AnimateCells(step->script, step->x, step->z);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 2;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004a9c();
}

void Func_02000940(s32 countdown) {
  Func_020008cc(gData_02005290, countdown, 0);
}

void Func_02000954(void) {}

void Func_02000958(void) { GameFlag_SetBit(0x8be); }

void Func_02000968(void) {
  struct Actor *leader;

  goto check;
run:
  if (Func_02004a84(151) != -1)
    return;
  Event_SetFlag(0x8bf);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(4, 488, 440);
  FieldActor_LaunchFromLeader(5, -16, 0, 0);
  Actor_FaceDirectionAndWait(4, FACING_WEST);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIDonTKnowWhatWe);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_SetAnimation(5, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_SetDestination(5, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Func_02004a9c();
  return;
check:
  if (Event_GetFlag(0x8ab) && Event_GetFlag(0x8a7) == 0 && Event_GetFlag(0x8be))
    goto run;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }
