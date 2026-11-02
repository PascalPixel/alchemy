#include "types.h"
#include "scene_tables.h"
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

  actor = Actor_GetById(16);
  Func_02003c28();
  Func_02003d78(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Func_02003cf8()->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(760), PIXELS(32), PIXELS(204), 1);
  Func_02003cf0();
  Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Value3((s32 (*)())Func_02003cc8, 16, 0x100, 40);
  Actor_SetSpeedParameters(16, 0x20000, 0x10000);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 760, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  FieldEvent_Wait(20);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 792, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Func_02002698(9, 16, 0x210);
  Func_02003df8(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 824, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  FieldEvent_Wait(20);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  FieldEvent_Wait(20);
  Func_02003c30();
}
