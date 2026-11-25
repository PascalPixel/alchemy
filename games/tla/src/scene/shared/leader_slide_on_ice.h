/// The leader slides cell by cell over ice until something stops them: a wall,
/// ground more than 8 pixels up, a solid actor, or the end of the ice.
void Leader_SlideOnIce(void) {
  struct Vector3 probe;
  s32 startBehavior;
  FIXED x;
  FIXED z;
  FIXED startX;
  FIXED startZ;
  u8 *layer;
  struct Actor *actor;
  s32 heading;
  s32 nextBehavior;
  FIXED distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  FIXED dx;
  FIXED dz;

  actor = Actor_Lookup(gMainRoutineState.leaderActorId);

top:
  Leader_StopIfPending();
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  actor->targetX = ACTOR_NO_TARGET;
  actor->targetZ = ACTOR_NO_TARGET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  probe.x = startX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  probe.y = actor->y.fixed;
  probe.z = startZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  x = startX;
  z = startZ;
  layer = &actor->layer;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z.fixed;
  dx = x - actor->x.fixed;
  distance = Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz);
  if (distance < toFIXED(4)) {
    heading = gSlideOnIce_AnglesByKey[DPAD_INDEX(gInput.held)];
    // -1 for no direction, tested as its halfword.
    if ((u16)heading == DPAD_ANGLE_NONE) {
      return;
    }
  } else {
    heading = (s16)Trig_Atan(z - actor->z.fixed, x - actor->x.fixed);
    heading = (s16)FACING_NEAREST_CARDINAL(heading);
  }
  FieldVector_AddPolarOffset(16, heading, &probe);
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, probe.x, probe.z);
  if (nextBehavior == CELL_BEHAVIOR_WALL ||
      Map_GetTerrainHeight(*layer, probe.x, probe.z) - actor->y.fixed >
          toFIXED(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x.fixed = startX;
    actor->z.fixed = startZ;
    goto done;
  }
  probe.x = startX;
  probe.z = startZ;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 10;
  actor->timer = 0;
  FieldActor_SetMoveTargetFX(actor, startX, actor->y.fixed, startZ);
  Actor_SelectAnimation(actor, ANIM_WALK);
  Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
  FieldActor_WaitForMove(actor);
  actor->update = Actor_SteerByPad;
  for (;;) {
    FieldVector_AddPolarOffset(16, heading, &probe);
    nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL) {
      break;
    }
    if (Map_GetTerrainHeight(*layer, probe.x, probe.z) - actor->y.fixed >
        toFIXED(8)) {
      break;
    }
    {
      s32 radius = actor->radius - 2;

      other = Ram_HeapSlots->actors;
      flags = &other->collisionFlags;
      for (i = 0; i < ACTOR_COUNT;
           i++, flags += sizeof(struct Actor), other++) {
        if (other->script != NULL && (*flags & ACTOR_SOLID) && other != actor) {
          if (Event_CheckSpheresOverlap(&other->x.fixed, radius, &probe.x,
                                        other->radius - 2) >= 0) {
            goto stop;
          }
        }
      }
    }
    x = probe.x;
    z = probe.z;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    FieldActor_SetMoveTargetFX(actor, probe.x, probe.y, probe.z);
    FieldActor_WaitForMove(actor);
    if (nextBehavior != startBehavior) {
      goto blocked;
    }
  }
stop:
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1);
  FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->acceleration = toFIXED(1) / 4;
  Actor_SelectAnimation(actor, ANIM_STAND);
}
