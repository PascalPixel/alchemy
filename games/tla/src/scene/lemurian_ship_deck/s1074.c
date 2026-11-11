#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 MsgWellThereS[];

void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fd4(s32 id, s32 target);

static __inline__ void SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so the arguments are set up in their own order and each call
   * keeps its constants; as a direct call, the costly constant is built first
   * or shared, 2 of 101 instructions differ in tla-en. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPosition(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so the arguments are set up in their own order and each call
   * keeps its constants; as a direct call, the costly constant is built first
   * or shared, 2 of 101 instructions differ in tla-en. */
  Actor_SetPositionAndReset(id, x, z);
}

void Func_02001074(void) {
  struct Actor *object;

  if (Event_GetFlag(0x212))
    return;
  if (Event_GetFlag(0x988))
    return;
  Event_SetFlag(0x212);
  if (Event_GetFlag(0x211)) {
    Func_02001f5c();
    FieldEvent_ResolvePendingActions(0);
    Func_02001fd4(9, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    SetSpeed(9, 0xcccc, 0x6666);
    SetPosition(9, 416, 428);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    FieldEvent_SetMessageId(MsgWellThereS);
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(9, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(9, *(s16 *)((u8 *)object + 10),
                           *(s16 *)((u8 *)object + 18));
    FieldActor_WaitForMoveById(9);
    Actor_SetPosition(9, 0, 0);
    Event_SetFlag(0x988);
    Func_02001f64();
  } else if (Event_GetFlag(0x210)) {
    Event_SetFlag(0x211);
  } else {
    Event_SetFlag(0x210);
  }
}
