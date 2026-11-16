/// The leader slides cell by cell over ice until something stops it.
void Leader_SlideOnIce(void) {
  struct Vector3 probe;
  s32 startBehavior;
  FIXED x;
  FIXED z;
  FIXED startX;
  FIXED startZ;
  u8 *layer;
  struct Actor *actor;
  struct Vector3 *position;
  struct Vector3 *next;
  s32 heading;
  s32 nextBehavior;
  FIXED distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  FIXED dx;
  FIXED dz;

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);

top:
  Leader_StopIfPending();
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  actor->targetX = TARGET_UNSET;
  actor->targetZ = TARGET_UNSET;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position->y = actor->y.fixed;
  position->z = startZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  x = startX;
  z = startZ;
  layer = &actor->layer;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z.fixed;
  dx = x - actor->x.fixed;
  distance = Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz);
  if (distance < toFIXED(4)) {
    heading = gSlideOnIce_AnglesByKey[(gInput.held >> 4) & 15];
    if ((heading << 16) == -65536) {
      return;
    }
  } else {
    heading = (s16)Trig_Atan(z - actor->z.fixed, x - actor->x.fixed);
    heading = (s16)((heading + DEGtoANG(45)) & -0x4000);
  }
  FieldVector_AddPolarOffset(16, heading, &probe);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y.fixed >
          toFIXED(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x.fixed = startX;
    actor->z.fixed = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 10;
  actor->timer = 0;
  FieldActor_SetMoveTargetFX(actor, startX, actor->y.fixed, startZ);
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  FieldActor_WaitForMove(actor);
  actor->update = Actor_SteerByPad;
  for (;;) {
    FieldVector_AddPolarOffset(16, heading, &probe);
    next = &probe;
    nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
    if (nextBehavior == 255) {
      break;
    }
    if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y.fixed >
        toFIXED(8)) {
      break;
    }
    {
      s32 radius = actor->radius - 2;

      other = Ram_HeapSlots->actors;
      flags = &other->collisionFlags;
      for (i = 0; i < 64; i++, flags += 128, other++) {
        if (other->script != NULL && (*flags & 1) && other != actor) {
          if (Event_CheckSpheresOverlap(&other->x.fixed, radius, &probe.x,
                                        other->radius - 2) >= 0) {
            goto stop;
          }
        }
      }
    }
    /* FAKEMATCH(dead): the form kept; as next set once, at the head of the
     * loop, 69 of 302 instructions differ in tla-en. */
    next = &probe;
    x = next->x;
    z = next->z;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    FieldActor_SetMoveTargetFX(actor, next->x, next->y, next->z);
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
  Actor_SelectAnimation(actor, 1);
}
