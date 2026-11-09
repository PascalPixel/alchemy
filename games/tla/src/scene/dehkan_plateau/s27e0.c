#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_020021b4();
void Func_02002698();
void Func_02003c28();
void Func_02003c30();
void Func_02003cf0();
void Actor_SetSingleChildFlags();

void Func_020027e0(void) {
  struct Actor *actor;

  actor = Actor_GetById(25);
  Func_02003c28();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  Func_080d4568()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(616), -1, PIXELS(280), 1);
  Func_02003cf0();
  Actor_SetAngleToward(25, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Func_080d47b4(25, 0x100, 40);
  Actor_SetSpritePriority(25, 1);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 632, 280);
  Actor_SetSpeed(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 664, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(21, 25, 0x210);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 696, 280);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_WalkToAndWait(25, 712, 280);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Camera_MoveWithinMapBounds(PIXELS(712), -1, PIXELS(136), 1);
  Map_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(25, 1);
  Actor_WalkToAndWait(25, 712, 200);
  Actor_SetPositionAndReset(25, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_ArmCallbackAndRefresh(25, FACING_EAST);
  Actor_SetSpeedParameters(25, 0x20000, 0x10000);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 168);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Func_02002698(24, 25, 0x216);
  Actor_ArmCallbackAndRefresh(25, FACING_NORTH);
  Sound_Play(153);
  actor->velocityY = 0x80000;
  Actor_SetSingleChildFlags(Actor_GetById(25), 0);
  Actor_WalkToAndWait(25, 744, 136);
  Actor_SetSingleChildFlags(Actor_GetById(25), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(25, 744, 72);
  Map_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(229);
  Func_020021b4();
  Event_Wait(10);
  Actor_Jump(25, 6, 16);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x870);
  Func_02003c30();
}
