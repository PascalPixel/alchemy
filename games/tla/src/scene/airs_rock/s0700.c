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
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  u32 i;

  Event_PlaySound(390);
  Func_02003b28(leader, 6);
  System_Wait(10);
  Func_02003b28(leader, 1);
  Actor_SetSingleChildFlags(leader, 0);
  leader->motionFlags |= 2;
  leader->velocityY = 0x40000;
  FieldActor_SetMoveTarget(leader, leader->x, leader->y, z + 0xc0000);
  System_Wait(6);
  Event_PlaySound(217);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x5000, 0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y += -0x20000;
    leader->targetY = leader->y;
    System_Wait(1);
  }
  leader->motionFlags = 3;
  leader->velocityY = 0x30000;
  FieldActor_SetMoveTarget(leader, leader->x, leader->y, z + 0x100000);
  Actor_WaitForMove(leader);
  Actor_SetSingleChildFlags(leader, 1);
  Event_PlaySound(390);
  Func_02003c98();
}
