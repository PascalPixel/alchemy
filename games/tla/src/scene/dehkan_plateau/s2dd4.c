#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

void Camera_MoveWithinMapBounds();
void FieldEvent_Wait();
void Func_02002698();
void Func_02003c28();
void Func_02003c30();
void Func_02003cc8();
void Func_02003cf0();
struct Actor *Func_02003cf8();
void Func_02003d78();
void Func_02003df8();
void Actor_SetSingleChildFlags();
void Actor_Stop();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();

void Func_02002dd4(void) {
  struct Actor *actor;

  actor = (struct Actor *)Value1((s32 (*)())Actor_GetById, 16);
  Func_02003c28();
  Call1(Func_02003d78, 0);
  Call2(Actor_WatchAndSetScript, gMainRoutineWork.currentOwner, 16);
  ((struct Actor *)Value0((s32 (*)())Func_02003cf8))->motionFlags = 0;
  Call4(Camera_MoveWithinMapBounds, 0x2f80000, 0x200000, 0xcc0000, 1);
  Func_02003cf0();
  Call3(Actor_SetAngleToward, 16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Value3((s32 (*)())Func_02003cc8, 16, 0x100, 40);
  Call3(Actor_SetSpeedParameters, 16, 0x20000, 0x10000);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 0);
  Call3(Actor_SetPositionAndReset, 16, 0x2f8, 200);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 1);
  Call1(FieldEvent_Wait, 20);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 0);
  Call3(Actor_SetPositionAndReset, 16, 0x318, 200);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 1);
  Call3(Func_02002698, 9, 16, 0x210);
  Call1(Func_02003df8, 153);
  actor->velocityY = 0x80000;
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 0);
  Call3(Actor_SetPositionAndReset, 16, 0x338, 200);
  Call2(Actor_SetSingleChildFlags,
        Value1((s32 (*)())Actor_GetById, 16), 1);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_WatchAndSetScript, 16, gMainRoutineWork.currentOwner);
  Call1(Actor_Stop, gMainRoutineWork.currentOwner);
  Call1(FieldEvent_Wait, 20);
  Func_02003c30();
}
