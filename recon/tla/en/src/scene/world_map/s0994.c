#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void wait(s32 frames);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 actor, s32 x, s32 z);
void Func_02004c98(s32 actor, s32 target);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004d00(s32 actor, s32 angle);
s32 Func_02004d08(s32 actor, s32 value, s32 frames);
void Func_02004d10(s32 actor, s32 value);
void Func_02004d40(s32 value);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e30(s32 cue);
struct Actor *Actor_GetById(s32 id);

/// Brings the actor to the player character's position.
static __inline__ void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(actor, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Func_02004c88(actor, 0, 0);
}

void Func_02000994(void) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 southwest = FACING_SOUTHWEST;
  s32 west = FACING_WEST;

  if (Flag_IsSet(SPLIT_CONSTANT(SPLIT_CONSTANT(0xa00, 33))) && !Flag_IsSet(0x9d0)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    x = object->x;
    z = object->z;
    x >>= 20;
    z >>= 20;
    Func_02004c20();
    Func_02004db8(0);
    if (Flag_IsSet(SPLIT_CONSTANT(0x9b3))) {
      Func_02004df8(SPLIT_CONSTANT(SPLIT_CONSTANT(0x16600, 102)), 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      wait(1);
      Actor_SetPositionAndReset(5, (x << 4) + 24, (z << 4) - 12);
      Actor_ArmCallback(5, southwest, 0);
      Func_02004d00(gMainRoutineWork.currentOwner, FACING_NORTHEAST);
      Func_02004da8();
      Func_02004cd8((s32)MsgWeShouldnT);
      Func_02004cf0(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      wait(1);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 7312, 8192);
      Func_02004df8(0x10000, 6);
    } else {
      s32 besideZ;

      Func_02004df8(SPLIT_CONSTANT(SPLIT_CONSTANT(0x16600, 102)), 6);
      Func_02004c98(5, gMainRoutineWork.currentOwner);
      wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_SetPositionAndReset(5, x + 24, besideZ);
      Actor_ArmCallback(5, southwest, 0);
      Func_02004d00(gMainRoutineWork.currentOwner, FACING_NORTHEAST);
      Func_02004da8();
      Func_02004cd8((s32)MsgYouReNot);
      Func_02004cf0(5, 0);
      Func_02004db0();
      Actor_ArmCallback(5, west, 0);
      Func_02004d10(gMainRoutineWork.currentOwner, 258);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, x + 12, besideZ);
      Func_02004d00(gMainRoutineWork.currentOwner, 0);
      Actor_SetModeById(5, 4);
      Func_02004da8();
      Func_02004cf0(5, 0);
      Func_02004d08(5, 265, 0);
      Func_02004cf0(5, 0);
      Actor_SetModeById(5, 3);
      Func_02004cf0(5, 0);
      Func_02004db0();
      Actor_JoinLeader(5);
      wait(1);
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 7312, 8192);
      Flag_Set(SPLIT_CONSTANT(0x900, 179));
      Func_02004df8(0x10000, 6);
    }
    wait(8);
    Func_02004c28();
  } else {
    struct EventDestination *work = Ram_HeapSlots->eventWork;

    Func_02004c20();
    Func_02004db8(0);
    Func_02004e30(123);
    Func_02004d40(work->scene);
  }
}
