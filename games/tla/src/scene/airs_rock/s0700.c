#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02003b28(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02003c98(void);

/// The leader jumps up 12 pixels (cue 390, mode 6 then 1), climbs 28 pixels
/// over 14 frames with cue 217 while turned to angle 0x5000, then lands 16
/// pixels on and plays cue 390 again.
void Func_02000700(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  u32 i;

  Sound_Play(390);
  Func_02003b28(leader, 6);
  wait(10);
  Func_02003b28(leader, 1);
  Actor_SetSingleChildFlags(leader, 0);
  leader->motionFlags |= 2;
  leader->velocityY = 0x40000;
  Actor_SetMoveTarget(leader, leader->x, leader->y, z + 0xc0000);
  wait(6);
  Sound_Play(217);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0x5000, 0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y += -0x20000;
    leader->targetY = leader->y;
    wait(1);
  }
  leader->motionFlags = 3;
  leader->velocityY = 0x30000;
  Actor_SetMoveTarget(leader, leader->x, leader->y, z + 0x100000);
  Script_WaitForEventTimeout(leader);
  Actor_SetSingleChildFlags(leader, 1);
  Sound_Play(390);
  Func_02003c98();
}
