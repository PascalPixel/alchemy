#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "flag_call.h"
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

  if (Flag_IsSet(0xa21) && !Flag_IsSet(0x9d0)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    x = object->x;
    z = object->z;
    x >>= 20;
    z >>= 20;
    Func_02004c20();
    Event_ResolvePendingActions(0);
    if (Flag_IsSet(0x9b3)) {
      Func_080d46a4(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      wait(1);
      Actor_SetPositionAndReset(5, (x << 4) + 24, (z << 4) - 12);
      Actor_ArmCallback(5, southwest, 0);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      Event_SetMessageId((s32)MsgWeShouldnT);
      Event_ShowMessageAndWait(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      wait(1);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 7312, 8192);
      Func_080d46a4(0x10000, 6);
    } else {
      s32 besideZ;

      Func_080d46a4(0x16666, 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_SetPositionAndReset(5, x + 24, besideZ);
      Actor_ArmCallback(5, southwest, 0);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner,
                                  FACING_NORTHEAST);
      Func_02004da8();
      Event_SetMessageId((s32)MsgYouReNot);
      Event_ShowMessageAndWait(5, 0);
      Func_02004db0();
      Actor_ArmCallback(5, west, 0);
      Func_080d489c(gMainRoutineWork.currentOwner, 258);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, 0);
      Actor_SetModeById(5, 4);
      Func_02004da8();
      Event_ShowMessageAndWait(5, 0);
      Func_080d47b4(5, 265, 0);
      Event_ShowMessageAndWait(5, 0);
      Actor_SetModeById(5, 3);
      Event_ShowMessageAndWait(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      wait(1);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 7312, 8192);
      Flag_Set(0x9b3);
      Func_080d46a4(0x10000, 6);
    }
    wait(8);
    Func_02004c28();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Func_02004c20();
    Event_ResolvePendingActions(0);
    Sound_Play(123);
    Import_EventSetExitRequest(work->scene);
  }
}
