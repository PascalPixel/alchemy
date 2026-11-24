/// Pushes the block ahead onto another, if nothing blocks the way.
void MagmaRockInside_PushBlockOnto(void) {
  struct FieldPosition dst;
  struct Actor *lead;
  struct Actor *next;
  struct Actor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(gMainRoutineState.leaderActorId);
  dir = lead->facing / FACING_STEP;
  // A push step holds x pixels in its high half and z in its low (PUSH_STEP).
  step = gPushLeaderSteps[dir];
  dst.x = lead->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = lead->y.fixed;
  step = INTtoFIXED(step);
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = next->y.fixed;
  step = INTtoFIXED(step);
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + toFIXED(16);
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    return;

  next->layer = MAPGRID_LAYER_OVERHEAD;
  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = next->y.fixed;
  step = INTtoFIXED(step);
  dst.z = next->z.fixed + step;
  if (MapGrid_CheckCollision(next, &dst) > 0)
    return;

  if (next->sceneCounter != 0)
    return;

  Actor_SelectAnimation(lead, ANIM_PUSH);
  rate = toFIXED(0.2);
  System_Wait(15);
  Event_PlaySound(SE_PUSH);
  next->speedLimit = rate;
  next->acceleration = rate;
  FieldActor_SetMoveTargetFX(next, dst.x, dst.y, dst.z);
  lead->speedLimit = rate;
  lead->acceleration = rate;
  FieldActor_SetMoveTargetFX(lead, dst.x, dst.y, dst.z);
  FieldActor_WaitForMove(next);
  Event_NoOp();
  next->x.fixed = dst.x;
  next->z.fixed = dst.z;
  next->velocityX = 0;
  next->velocityZ = 0;
  lead->targetX = ACTOR_NO_TARGET;
  lead->targetZ = ACTOR_NO_TARGET;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = INTtoFIXED(lead->x.part[0].integer);
  lead->z.fixed = INTtoFIXED(lead->z.part[0].integer);
  Actor_SelectAnimation(lead, ANIM_STAND);
}
