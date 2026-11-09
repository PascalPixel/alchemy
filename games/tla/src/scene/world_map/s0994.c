#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c98(s32 actor, s32 target);
void Func_02004da8(void);
void Func_02004db0(void);

/// Brings the actor to the player character's position.
static __inline__ void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

void Func_02000994(void) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 southwest = FACING_SOUTHWEST;
  s32 west = FACING_WEST;

  if (Event_GetFlag(0xa21) && !Event_GetFlag(0x9d0)) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    x = object->x;
    z = object->z;
    x >>= 20;
    z >>= 20;
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0x9b3)) {
      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      Actor_WalkToAndWait(5, (x << 4) + 24, (z << 4) - 12);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      FieldEvent_SetMessageId((s32)MsgWeShouldnT);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    } else {
      s32 besideZ;

      FieldEffect_ScheduleRatioTransition(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      System_Wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_WalkToAndWait(5, x + 24, besideZ);
      Actor_FaceDirection(5, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      FieldEvent_SetMessageId((s32)MsgYouReNot);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_FaceDirection(5, west, 0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0);
      Actor_SetAnimation(5, 4);
      Func_02004da8();
      Event_ShowMessage(5, 0);
      Actor_ShowEmote(5, 265, 0);
      Event_ShowMessage(5, 0);
      Actor_SetAnimation(5, 3);
      Event_ShowMessage(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 7312, 8192);
      Event_SetFlag(0x9b3);
      FieldEffect_ScheduleRatioTransition(0x10000, 6);
    }
    System_Wait(8);
    Func_02004c28();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(work->scene);
  }
}
