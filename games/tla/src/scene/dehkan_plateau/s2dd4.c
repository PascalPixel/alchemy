#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02002698();
void Func_02003c28();
void Func_02003c30();
void Func_02003cf0();
void Actor_SetSingleChildFlags();

void Func_02002dd4(void) {
  struct Actor *actor;

  actor = Actor_GetById(16);
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  Func_080d4568()->motionFlags = 0;
  Camera_MoveWithinMapBounds(PIXELS(760), PIXELS(32), PIXELS(204), 1);
  Func_02003cf0();
  Actor_SetAngleToward(16, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(16, 0x100, 40);
  Actor_SetSpeedParameters(16, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 760, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 792, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Func_02002698(9, 16, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetPositionAndReset(16, 824, 200);
  Actor_SetSingleChildFlags(Actor_GetById(16), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Event_Wait(20);
  Func_02003c30();
}
