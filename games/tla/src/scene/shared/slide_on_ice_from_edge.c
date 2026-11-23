/// The leader slides over ice from the cell they stand on: from its centre in
/// the direction the pad holds, or else from its edge toward the centre, then
/// cell by cell until a wall, a raised cell or another actor stops them.
void Leader_SlideOnIceFromEdge(void) {
  struct Actor *actor;
  s32 startCell;
  FIXED targetX;
  FIXED targetZ;
  FIXED x;
  FIXED z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  FIXED dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;

  // gSlideOnIce_Stopped is volatile: a task of the VBlank pass sets it while
  // the slide waits (Leader_SetSlideStopped), and each test reads it again.
  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  gSlideOnIce_Stopped = 0;
retry:
  Leader_StopIfPending();
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position[0] = x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[1] = actor->y.fixed;
  position[2] = z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  targetZ = z;
  targetX = x;
  layer = &actor->layer;
  startCell = MapGrid_GetCellBehaviorAt(*layer, targetX, targetZ);
  dz = targetZ - actor->z.fixed;
  dx = targetX - actor->x.fixed;
  if (Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz) < toFIXED(4)) {
    angle = gSlideOnIce_AnglesByKey[DPAD_INDEX(gInput.held)];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == DPAD_ANGLE_NONE)
#if !TLA_PATCH_1
      return;
#else
      goto end;
#endif
  } else {
    angle = Trig_Atan(targetZ - actor->z.fixed, targetX - actor->x.fixed);
    angle = FACING_NEAREST_CARDINAL(angle);
  }
  FieldVector_AddPolarOffset(16, angle, position);
  cell = MapGrid_GetCellBehaviorAt(*layer, position[0], position[2]);
  if (cell == CELL_BEHAVIOR_WALL ||
      Map_GetTerrainHeight(*layer, position[0], position[2]) - actor->y.fixed >
          toFIXED(8)) {
    actor->facing = angle;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x.fixed = x;
    actor->z.fixed = z;
    goto end;
  }
  position[0] = x;
  position[2] = z;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 10;
  actor->timer = 0;
  Actor_SetMoveTarget(actor, x, actor->y.fixed, z);
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
#if !TLA_PATCH_1
  FieldActor_WaitForMove(actor);
#else
  for (i = 0; i <= 179 && Actor_HasArrived(actor) == 0; i++) {
    if (gSlideOnIce_Stopped != 0)
      goto end;
    System_Wait(1);
  }
#endif
  actor->update = (ObjectUpdate)Actor_SteerByPad;
  for (;;) {
    FieldVector_AddPolarOffset(16, angle, position);
    q = position;
    cell = MapGrid_GetCellBehaviorAt(*layer, q[0], q[2]);
    if (cell == CELL_BEHAVIOR_WALL)
      break;
    if (Map_GetTerrainHeight(*layer, q[0], q[2]) - actor->y.fixed > toFIXED(8))
      goto blocked;
    if (gSlideOnIce_Stopped != 0)
#if !TLA_PATCH_1
      goto blocked;
#else
      goto end;
#endif
    radius = actor->radius - 2;
    entry = Ram_HeapSlots->actors;
    flags = &entry->collisionFlags;
    for (j = 0; j < ACTOR_COUNT; j++, flags += sizeof(*entry), entry++) {
      if (entry->script != NULL && (ACTOR_SOLID & *flags) && entry != actor) {
        if (Event_CheckSpheresOverlap(&entry->x.fixed, radius, position,
                                      entry->radius - 2) >= 0)
          goto blocked;
      }
    }
    targetX = position[0];
    targetZ = position[2];
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    Actor_SetMoveTarget(actor, position[0], position[1], position[2]);
    for (i = 0;
         i <= 179 && Actor_HasArrived(actor) == 0 && gSlideOnIce_Stopped == 0;
         i++)
      System_Wait(1);
    if (gSlideOnIce_Stopped != 0)
      goto end;
    if (cell != startCell)
      goto changed;
  }
blocked:
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1);
  Actor_SetMoveTarget(actor, targetX, actor->y.fixed, targetZ);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->acceleration = toFIXED(1) / 4;
  Actor_SelectAnimation(actor, ANIM_STAND);
}
