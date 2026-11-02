#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

void Camera_MoveWithinMapBounds();
void FieldEvent_Wait();
void Func_020021b4();
void Func_02002698();
void Func_02003be0();
void Func_02003c28();
void Func_02003c30();
void Func_02003c68();
void Func_02003cb0();
void Func_02003cc8();
void Func_02003ce0();
void Func_02003cf0();
struct Actor *Func_02003cf8();
void Func_02003d78();
void Func_02003df8();
void GameFlag_SetBit();
void Actor_SetSingleChildFlags();
void Actor_Stop();
void Actor_Jump();
void Actor_SetSpritePriority();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();

void Func_020027e0(void) {
  struct Actor *actor;

  actor = Actor_GetById(25);
  Func_02003c28();
  Func_02003d78(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  Func_02003cf8()->motionFlags = 0;
  Call4(Camera_MoveWithinMapBounds, PIXELS(616), -1, PIXELS(280), 1);
  Func_02003cf0();
  Actor_SetAngleToward(25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Func_02003cc8(25, 0x100, 40);
  Actor_SetSpritePriority(25, 1);
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 25, 632, 280);
  Call3(Actor_SetSpeedParameters, 25, 0x20000, 0x10000);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Call3(Actor_SetPositionAndReset, 25, 664, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(21, 25, 0x210);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Call3(Actor_SetPositionAndReset, 25, 696, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  FieldEvent_Wait(10);
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 25, 712, 280);
  Func_02003ce0(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(712), -1, PIXELS(136), 1);
  Func_02003be0(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(25, 1);
  Call3(Actor_SetPositionAndReset, 25, 712, 200);
  Actor_SetPositionAndReset(25, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Func_02003cb0(25, FACING_EAST);
  Actor_SetSpeedParameters(25, 0x20000, 0x10000);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Call3(Actor_SetPositionAndReset, 25, 744, 168);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(24, 25, 0x216);
  Func_02003cb0(25, FACING_NORTH);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Call3(Actor_SetPositionAndReset, 25, 744, 136);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  FieldEvent_Wait(10);
  Call3(Actor_SetPositionAndReset, 25, 744, 72);
  Func_02003be0(66, 51, 110, 4, 1, 1);
  Func_02003df8(229);
  Func_020021b4();
  FieldEvent_Wait(10);
  Actor_Jump(25, 6, 16);
  Func_02003c68(25, 0, 0);
  FieldEvent_Wait(20);
  GameFlag_SetBit(0x870);
  Func_02003c30();
}
