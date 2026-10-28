#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(u32 id);
void Func_02003cd8(s32 cue);
void Func_02003b28(struct Actor *object, s32 mode);
void WaitFrames(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02003b58(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003b60(struct Actor *object);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02003c98(void);

/// The leader jumps up 12 pixels (cue 390, mode 6 then 1), climbs 28 pixels
/// over 14 frames with cue 217 while turned to angle 0x5000, then lands 16
/// pixels on and plays cue 390 again.
void Func_02000700(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 z = leader->z;
  u32 i;

  Func_02003cd8(390);
  Func_02003b28(leader, 6);
  WaitFrames(10);
  Func_02003b28(leader, 1);
  Actor_SetSingleChildFlags(leader, 0);
  leader->motionFlags |= 2;
  leader->velocityY = 0x40000;
  Func_02003b58(leader, leader->x, leader->y, z + 0xc0000);
  WaitFrames(6);
  Func_02003cd8(217);
  Actor_ArmCallback(gAdventure.currentOwner, 0x5000, 0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y += -0x20000;
    leader->targetY = leader->y;
    WaitFrames(1);
  }
  leader->motionFlags = 3;
  leader->velocityY = 0x30000;
  Func_02003b58(leader, leader->x, leader->y, z + 0x100000);
  Func_02003b60(leader);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02003cd8(390);
  Func_02003c98();
}
