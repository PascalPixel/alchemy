#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 MsgWellThereS[];

#include "flag_call.h"
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fd4(s32 id, s32 target);

static __inline__ void SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void SetPosition(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Actor_SetPositionAndReset(id, x, z);
}

void Func_02001074(void) {
  struct Actor *object;

  if (Flag_IsSet(0x212))
    return;
  if (Flag_IsSet(0x988))
    return;
  Flag_Set(0x212);
  if (Flag_IsSet(0x211)) {
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
    Flag_Set(0x988);
    Func_02001f64();
  } else if (Flag_IsSet(0x210)) {
    Flag_Set(0x211);
  } else {
    Flag_Set(0x210);
  }
}
