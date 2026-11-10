#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "version.h"
#include "scene_tables.h"
#include "event_inline.h"

extern s32 gData_020024b4;
extern s32 gData_020024b0;
extern const u8 gData_0200218c[];
extern const u8 gData_020021e8[];
extern const u8 gData_0200221c[];

void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000188(void);
void Func_020001cc(void);
void Func_02000258(void);
void Func_02000288(void);
void Func_020002cc(void);
void Func_020006e4(void);
void Func_02001f1c(void);
void Func_02001f5c(void);
void Func_0200206c(void);

static __inline__ void AddTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Task_Add(func, priority);
}

/// The scene run from entrance 10: the crew takes its places on deck and the
/// leader is asked to take the tiller; it ends by setting flag 0x8de.
void Func_020002cc(void) {
  s32 angle;
  s32 turn;

  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(342), PIXELS(48), PIXELS(408), 0);
  System_Wait(1);
  MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3);
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, PIXELS(552), PIXELS(376),
                FACING_EAST);
  /* FAKEMATCH: the two-instruction angle set before its call is built ahead
   * of the position arguments; as a constant argument, 4 instructions differ
   * in tla-en. */
  angle = 0xd000;
  Actor_SetPositionAndFacing(9, PIXELS(558), PIXELS(345), angle);
  Actor_SetPositionAndFacing(7, PIXELS(534), PIXELS(404), FACING_EAST);
  turn = 0x2000;
  Actor_SetPositionAndFacing(6, PIXELS(568), PIXELS(408), turn);
  angle = 0x4000;
  Actor_SetPositionAndFacing(5, PIXELS(546), PIXELS(440), angle);
  Func_02001f1c();
  System_Wait(1);
  Actor_SetScript(5, gData_0200218c);
  Actor_SetScript(6, gData_020021e8);
  Actor_SetScript(9, gData_0200221c);
  Event_StartScreenTransition();
  gData_020024b4 = 0;
  Task_AddFrame(Func_02000258, 0x480);
  gData_020024b0 = 0;
  Task_AddFrame(Func_02000288, 0x480);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(542), PIXELS(44), PIXELS(396), 1);
  Func_0200206c();
  FieldEvent_Wait(80);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(7, 0);
  FieldActor_Stop(5);
  FieldActor_Stop(9);
  FieldActor_Stop(6);
  System_Wait(1);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_ShowEmote(6, 0x101, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, turn);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x108, 20);
  Event_ShowMessage(5, 0);
  Event_OpenMessage(7, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, 0x109, 20);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
    Actor_ShowEmote(9, 0x109, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Event_ShowMessage(9, 0);
  }
  Actor_ShowEmote(7, 0x100, 0);
  Event_ShowMessage(7, 0);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(6, 0x101, 20);
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x100, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(5, 0x108, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(6, 0x102);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Event_ShowMessage(0x4006, 0);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_ShowEmote(7, 0x101, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 0x108, 20);
  Event_ShowMessage(9, 0);
  FieldActor_Jump(6, 4, 10);
  Event_ShowMessage(6, 0);
  FieldActor_Jump(5, 2, 10);
  FieldActor_Jump(5, 4, 10);
  Event_ShowMessage(5, 0);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(9, 3);
  Event_ShowMessage(9, 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.renderMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(0x8de);
  FieldEvent_SetExitRequest(10);
}

/// Sends actor id to actor 4's spot at a run.
static inline void RunToActor4(s32 id) {
  struct Actor *target;

  Actor_SetSpeedParameters(id, 0x13333, 0x9999);
  Actor_SetModeById(id, 2);
  target = Actor_GetById(4);
  if (target != NULL)
    Actor_ResetAndSetPosition(id, ((s16 *)&target->x)[1],
                              ((s16 *)&target->z)[1]);
  Actor_WaitForMoveById(id);
  Actor_SetHorizontalPositionWithTerrain(id, 0, 0);
}

/// The scene run from entrance 20: the camera frames actors 4 to 9 around the
/// spot, two callbacks start the bobbing, and after the exchange of looks and
/// emotes the four actors run to actor 4.
void Func_020006e4(void) {
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(458), PIXELS(48), PIXELS(408), 0);
  FieldEvent_SetMessageId(MsgHoldOnTo);
  Actor_SetPositionAndFacing(4, PIXELS(470), PIXELS(392), FACING_EAST);
  FieldActor_LaunchFromLeader(6, 0, -24, FACING_SOUTH);
#if !TLA_PATCH_2
  FieldActor_LaunchFromLeader(5, -16, -16, FACING_SOUTHEAST);
#else
  {
    /* FAKEMATCH: from patch 2 the game builds -16 once in r1 and copies it to
     * r2; as two constants it is built twice, and as one variable (copied,
     * chained or in an array) it takes r2 and is copied to r1 (differ 6). */
    register s32 offset asm("r1") = -16;

    FieldActor_LaunchFromLeader(5, offset, offset, FACING_SOUTHEAST);
  }
#endif
  FieldActor_LaunchFromLeader(9, -24, 8, FACING_NORTHEAST + FACING_STEP);
  FieldActor_LaunchFromLeader(7, -4, 24, FACING_NORTHEAST);
  MapGrid_CopyCellsTo(60, 53, 67, 16, 7, 3);
  Func_02001f1c();
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gData_020024b4 = 0;
  Task_AddFrame(Func_02000258, 0x480);
  gData_020024b0 = 0;
  Task_AddFrame(Func_02000288, 0x480);
  Actor_ShowEmote(6, 0x102, 30);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_ShowEmote(4, 0x102, 30);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  FieldEvent_Wait(30);
  Actor_FaceDirectionAndWait(5, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(6, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(6, FACING_SOUTH);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_ShowEmote(7, 0x109, 30);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_SOUTHEAST);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(7, 0x102, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_FaceEachOther(6, 5, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(7, 0, 5);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHEAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(9, 4);
  FieldEvent_ShowMessageThenWait(9, 0, 5);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  Actor_FaceEachOther(9, 7, 0);
  Actor_FaceEachOther(6, 5, 0);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(9, 3);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(40);
  RunToActor4(7);
  RunToActor4(5);
  RunToActor4(6);
  RunToActor4(9);
  FieldEvent_SetExitRequest(1);
}

void Func_02000b84(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  AddTask(Func_02000188, 0x480);
  AddTask(Func_020001cc, 0x480);
  if (gMainRoutineWork.entrance == 10)
    Func_020002cc();
  if (gMainRoutineWork.entrance == 20)
    Func_020006e4();
}
