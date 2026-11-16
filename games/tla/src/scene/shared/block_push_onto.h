/// Pushes the block ahead onto another, if nothing blocks the way.
void MagmaRockInside_PushBlockOnto(void) {
  struct FieldPosition dst;
  struct Actor *lead;
  struct Actor *next;
  struct Actor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(gMainRoutineWork.leaderActorId);
  dir = lead->facing >> 12;
  step = gPushLeaderSteps[dir];
  dst.x = lead->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = next->y.fixed;
  step <<= 16;
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

  next->layer = 2;
  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & FIELD_PIXEL_MASK);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (MapGrid_CheckCollision(next, &dst) > 0)
    return;

  if (next->riseCounter != 0)
    return;

  Actor_SelectAnimation(lead, 8);
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
  lead->targetX = TARGET_UNSET;
  lead->targetZ = TARGET_UNSET;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part[0].integer << 16;
  lead->z.fixed = lead->z.part[0].integer << 16;
  Actor_SelectAnimation(lead, 1);
}
