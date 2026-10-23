/// @file
/// Vault, before and after the thieves are caught: villagers who walk about
/// stop when someone is close in front of them.
#include "types.h"
#include "object_event.h"

/// Stops the object event (paused, standing) when the target is nearer than
/// range pixels and lies ahead of it, within a sixteenth of a turn either side
/// of its heading; force stops it regardless. Returns 1 when it stopped. A
/// target out of range sets it walking again.
s32 Vault_StopIfFacingNear(struct ObjectEvent *objectEvent,
                           struct ObjectEvent *target, s32 range, s32 force) {
  s32 result = 0;
  const u8 *targetPosition =
      (const u8 *)target + offsetof(struct ObjectEvent, x);
  const u8 *objectEventPosition =
      (const u8 *)objectEvent + offsetof(struct ObjectEvent, x);
  if (FixedPoint_Distance(targetPosition, objectEventPosition) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z.fixed - objectEvent->z.fixed,
                             *(const s32 *)targetPosition -
                                 *(const s32 *)objectEventPosition);
    u32 left = (angle - FACING_STEP) & 0xf000;
    u32 right = (angle + FACING_STEP) & 0xf000;
    u32 forward = angle & 0xf000;
    u32 facing = objectEvent->facing & 0xf000;
    if (forward == facing || right == facing || left == facing || force != 0) {
      objectEvent->paused = 1;
      ObjectEvent_SetMode(objectEvent, ANIM_STAND);
      result = 1;
    }
  } else {
    objectEvent->paused = 0;
    ObjectEvent_SetMode(objectEvent, ANIM_WALK);
  }
  return result;
}
