/// @file
/// Vault, before and after the thieves are caught: villagers who walk about
/// stop when someone is close in front of them.
#include "types.h"
#include "field_obj.h"

/// Stop the actor (paused, mode 1) when the target is nearer than range pixels
/// and lies ahead of it, within a sixteenth of a turn either side of its
/// heading; force stops it regardless. Returns 1 when it stopped. A target out
/// of range sets it walking again (mode 2).
s32 Vault_StopIfFacingNear(struct FieldActor *actor, struct FieldActor *target,
                           s32 range, s32 force) {
  s32 result = 0;
  s32 *targetPosition = &target->x.fixed;
  s32 *actorPosition = &actor->x.fixed;
  if (FixedPoint_Distance(targetPosition, actorPosition) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                             *targetPosition - *actorPosition);
    u32 left = (angle - FACING_STEP) & 0xf000;
    u32 right = (angle + FACING_STEP) & 0xf000;
    u32 forward = angle & 0xf000;
    u32 facing = actor->facing & 0xf000;
    if (forward == facing || right == facing || left == facing || force != 0) {
      actor->paused = 1;
      Object_SetMode(actor, 1);
      result = 1;
    }
  } else {
    actor->paused = 0;
    Object_SetMode(actor, 2);
  }
  return result;
}
