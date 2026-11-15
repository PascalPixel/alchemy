/// @file
/// Vault, before and after the thieves are caught: villagers who walk about
/// stop when someone is close in front of them.

/// Stops the actor (paused, standing) when the target is nearer than
/// range pixels and lies ahead of it, within a sixteenth of a turn either side
/// of its heading; force stops it regardless. Returns 1 when it stopped. A
/// target out of range sets it walking again.
s32 Vault_StopIfFacingNear(struct Actor *actor, struct Actor *target, s32 range,
                           s32 force) {
  s32 result = 0;
  if (Actor_Distance(target, actor) < range || force != 0) {
    u32 angle = (u16)Trig_Atan(target->z.fixed - actor->z.fixed,
                               target->x.fixed - actor->x.fixed);
    u32 left = FACING_SIXTEENTH(angle - FACING_STEP);
    u32 right = FACING_SIXTEENTH(angle + FACING_STEP);
    u32 forward = FACING_SIXTEENTH(angle);
    u32 facing = FACING_SIXTEENTH(actor->facing);
    if (forward == facing || right == facing || left == facing || force != 0) {
      actor->paused = 1;
      Actor_SelectAnimation(actor, ANIM_STAND);
      result = 1;
    }
  } else {
    actor->paused = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
  }
  return result;
}
