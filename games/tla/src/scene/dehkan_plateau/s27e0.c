#include "types.h"
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

  actor = (struct Actor *)Value1((s32 (*)())Actor_GetById, 25);
  Func_02003c28();
  Call1(Func_02003d78, 0);
  Call2(Actor_WatchAndSetScript, gMainRoutineWork.currentOwner, 25);
  ((struct Actor *)Value0((s32 (*)())Func_02003cf8))->motionFlags = 0;
  Call4(Camera_MoveWithinMapBounds, 0x2680000, -1, 0x1180000, 1);
  Func_02003cf0();
  Call3(Actor_SetAngleToward, 25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Func_02003cc8(25, 0x100, 40);
  Call2(Actor_SetSpritePriority, 25, 1);
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 25, 0x278, 0x118);
  Call3(Actor_SetSpeedParameters, 25, 0x20000, 0x10000);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 0);
  Call3(Actor_SetPositionAndReset, 25, 0x298, 0x118);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 1);
  Call3(Func_02002698, 21, 25, 0x210);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 0);
  Call3(Actor_SetPositionAndReset, 25, 0x2b8, 0x118);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 1);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call3(Actor_SetPositionAndReset, 25, 0x2c8, 0x118);
  Call2(Func_02003ce0, 0x26666, 0x4ccc);
  Call4(Camera_MoveWithinMapBounds, 0x2c80000, -1, 0x880000, 1);
  Call6(Func_02003be0, 112, 6, 110, 4, 1, 1);
  Call2(Actor_SetSpritePriority, 25, 1);
  Call3(Actor_SetPositionAndReset, 25, 0x2c8, 200);
  Call3(Actor_SetPositionAndReset, 25, 0x2c8, 168);
  Call1(Actor_Stop, gMainRoutineWork.currentOwner);
  Call2(Func_02003cb0, 25, 0);
  Call3(Actor_SetSpeedParameters, 25, 0x20000, 0x10000);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 0);
  Call3(Actor_SetPositionAndReset, 25, 0x2e8, 168);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 1);
  Call3(Func_02002698, 24, 25, 0x216);
  Call2(Func_02003cb0, 25, 0xc000);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 0);
  Call3(Actor_SetPositionAndReset, 25, 0x2e8, 136);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 25), 1);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetPositionAndReset, 25, 0x2e8, 72);
  Call6(Func_02003be0, 66, 51, 110, 4, 1, 1);
  Call1(Func_02003df8, 229);
  Func_020021b4();
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_Jump, 25, 6, 16);
  Call3(Func_02003c68, 25, 0, 0);
  Call1(FieldEvent_Wait, 20);
  Call1(GameFlag_SetBit, 0x870);
  Func_02003c30();
}
