#include "types.h"
#include "adventure.h"
#include "actor.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait();
void Func_02001080();
void Func_02001088();
void Func_020011e0();
void Func_02001160();
void Func_02001168();
void Func_02001188();
void GameFlag_SetBit();
void GameFlag_Test();
void Actor_Jump();
void Actor_OffsetPositionAndResetMotion();
void Actor_OffsetPositionAndReset();
void Actor_WaitForMoveById();
void Actor_StartRepeatedMotion();
void Actor_ArmCallback();

/// Pushes Shaman Village cave's object 17 away from the leader, unless the two
/// stand on opposite sides of the boundary between 8-pixel columns 36 and 37.
/// With flag 514 set and the leader in the upper half (z cell 95 or less) it
/// slides down two cells, turns, settles with emote 258 and sets flag 516, only
/// once; otherwise it slides two cells along z, one and then three along x
/// away from the leader with a hop, and back along z.
void Func_02000184(void) {
  struct Actor *object = Actor_GetById(17);
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
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
  if (dz == 1 && Value1((s32 (*)())GameFlag_Test, 514) != 0) {
    if (Value1((s32 (*)())GameFlag_Test, 516) != 0)
      return;
    Func_02001080();
    Call1(Func_020011e0, 0);
    Call3(Actor_Jump, 17, 4, 20);
    Call3(Actor_OffsetPositionAndResetMotion, 17, 0, 32);
    Call1(Actor_WaitForMoveById, 17);
    Call2(Func_02001188, 17, 1);
    Call2(Actor_StartRepeatedMotion, 17, 2);
    Call3(Actor_ArmCallback, 17, 0x8000, 0);
    Call3(Actor_OffsetPositionAndResetMotion, 17, -8, 0);
    Call1(Actor_WaitForMoveById, 17);
    Call3(Actor_Jump, 17, 4, 20);
    Call2(Func_02001168, 17, 258);
    Call1(FieldEvent_Wait, 30);
    Actor_GetById(17)->actionFlags &= ~1;
    Call3(Actor_OffsetPositionAndReset, 17, 8, 0);
    Call1(Actor_WaitForMoveById, 17);
    Call1(FieldEvent_Wait, 30);
    Call3(Func_02001160, 17, 258, 0);
    Call1(FieldEvent_Wait, 30);
    Func_02001088();
    Call1(GameFlag_SetBit, 516);
    return;
  }
  Func_02001080();
  Call1(Func_020011e0, 0);
  dz <<= 5;
  Call3(Actor_Jump, 17, 4, 20);
  Call3(Actor_OffsetPositionAndResetMotion, 17, 0, dz);
  Call1(Actor_WaitForMoveById, 17);
  Call3(Actor_OffsetPositionAndResetMotion, 17, dx << 4, 0);
  Call1(Actor_WaitForMoveById, 17);
  Call3(Actor_OffsetPositionAndResetMotion, 17, dx * 48, 0);
  object->velocityY = 0x80000;
  dz = -dz;
  Call1(Actor_WaitForMoveById, 17);
  Call3(Actor_OffsetPositionAndResetMotion, 17, 0, dz);
  Call1(Actor_WaitForMoveById, 17);
  Func_02001088();
}
