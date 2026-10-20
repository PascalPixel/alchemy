/// @file
/// Vault, before and after the thieves are caught: villagers who walk about
/// stop when someone is close in front of them.
#include "types.h"
#include "field_obj.h"

/// Stops the actor (paused, mode 1) when the target is nearer than range pixels
/// and lies ahead of it, within a sixteenth of a turn either side of its
/// heading; force stops it regardless. Returns 1 when it stopped. A target out
/// of range sets it walking again (mode 2).
s32 Vault_StopIfFacingNear(struct FieldActor *actor, struct FieldActor *target,
                           s32 range, s32 force) {
  s32 result = 0;
  const u8 *targetPosition =
      (const u8 *)target + offsetof(struct FieldActor, x);
  const u8 *actorPosition = (const u8 *)actor + offsetof(struct FieldActor, x);
  if (FixedPoint_Distance(targetPosition, actorPosition) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                             *(const s32 *)targetPosition -
                                 *(const s32 *)actorPosition);
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
