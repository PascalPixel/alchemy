#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001080();
void Func_02001088();
void Func_02001188();

/// Pushes Shaman Village cave's object 17 away from the leader, unless the two
/// stand on opposite sides of the boundary between 8-pixel columns 36 and 37.
/// With flag 514 set and the leader in the upper half (z cell 95 or less) it
/// slides down two cells, turns, settles with emote 258 and sets flag 516, only
/// once; otherwise it slides two cells along z, one and then three along x
/// away from the leader with a hop, and back along z.
void Func_02000184(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 leaderX = leader->x >> 19;
  s32 leaderZ = leader->z >> 19;
  s32 objectX = object->x >> 19;
  s32 dx;
  s32 dz;

  if (leaderX <= 36 && objectX > 37)
    return;
  if (leaderX > 37 && objectX <= 36)
    return;
  dx = 1;
  if (leaderX > 36)
    dx = -dx;
  dz = -1;
  if (leaderZ <= 95)
    dz = 1;
  if (dz == 1 && Event_GetFlag(514) != 0) {
    if (Event_GetFlag(516) != 0)
      return;
    Func_02001080();
    Event_ResolvePendingActions(0);
    Actor_Jump(17, 4, 20);
    Actor_OffsetPositionAndResetMotion(17, 0, 32);
    Actor_WaitForMoveById(17);
    Func_02001188(17, 1);
    Actor_StartRepeatedMotion(17, 2);
    Actor_FaceDirection(17, FACING_WEST, 0);
    Actor_OffsetPositionAndResetMotion(17, -8, 0);
    Actor_WaitForMoveById(17);
    Actor_Jump(17, 4, 20);
    Actor_SetAttachedEffect(17, 258);
    Event_Wait(30);
    Actor_GetById(17)->actionFlags &= ~1;
    Actor_OffsetPositionAndReset(17, 8, 0);
    Actor_WaitForMoveById(17);
    Event_Wait(30);
    Func_080d47b4(17, 258, 0);
    Event_Wait(30);
    Func_02001088();
    GameFlag_SetBit(516);
    return;
  }
  Func_02001080();
  Event_ResolvePendingActions(0);
  dz <<= 5;
  Actor_Jump(17, 4, 20);
  Actor_OffsetPositionAndResetMotion(17, 0, dz);
  Actor_WaitForMoveById(17);
  Actor_OffsetPositionAndResetMotion(17, dx << 4, 0);
  Actor_WaitForMoveById(17);
  Actor_OffsetPositionAndResetMotion(17, dx * 48, 0);
  object->velocityY = 0x80000;
  dz = -dz;
  Actor_WaitForMoveById(17);
  Actor_OffsetPositionAndResetMotion(17, 0, dz);
  Actor_WaitForMoveById(17);
  Func_02001088();
}
