/// Pushes the block ahead onto another, if nothing blocks the way.
void MagmaRockInside_PushBlockOnto(void) {
  struct FieldPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = (struct StagedActor *)Actor_GetById(gMainRoutineWork.currentOwner);
  dir = lead->facing >> 12;
  step = gPushLeaderSteps[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gPushLeaderSteps[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (MapGrid_CheckCollision(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Actor_SelectAnimation(lead, 8);
  rate = 0x3333;
  System_Wait(15);
  Event_PlaySound(SE_PUSH);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  FieldActor_SetMoveTargetFixed(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  FieldActor_SetMoveTargetFixed(lead, dst.x, dst.y, dst.z);
  FieldActor_WaitForMove(next);
  Event_NoOp();
  next->x.fixed = dst.x;
  {
    /* FAKEMATCH(temporary): z read ahead of velocityX's store. */
    s32 z = dst.z;

    next->velocityX = 0;
    next->z.fixed = z;
    next->velocityZ = 0;
  }
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part.pixel << 16;
  lead->z.fixed = lead->z.part.pixel << 16;
  Actor_SelectAnimation(lead, 1);
}
