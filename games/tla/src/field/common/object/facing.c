/// @file
/// Objects: turn toward the linked target.

#include "types.h"
#include "facing_object.h"

s32 ArcTan2(s32, s32);

/// If a facing target exists, turn toward it by at most 0x1000 (22.5 degrees).
/// obj: object to turn.
s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *obj) {
  s32 turn;
  u16 old;
  s32 dir;
  struct FacingObject *target;

  target = obj->facingTarget;
  if (target != NULL) {
    obj->facingFlags = 0xfe & obj->facingFlags;
    dir = (u16)ArcTan2(target->positionZ - obj->positionZ,
                       target->positionX - obj->positionX);
    old = obj->facing;
    turn = (s16)(dir - old);
    if (turn != 0) {
      if (turn > 0x1000)
        turn = 0x1000;
      if (turn < -0x1000)
        turn = -0x1000;
      obj->facing = old + turn;
    }
  }
  return 1;
}
