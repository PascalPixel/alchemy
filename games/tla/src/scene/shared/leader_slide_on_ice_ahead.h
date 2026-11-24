/// The leader slides cell by cell over ice the way they face, turned to the
/// nearest cardinal, until a wall, ground more than 8 pixels up, a solid actor
/// or the end of the ice stops them.
void Leader_SlideOnIceAhead(void) {
  struct Vector3 probe;
  s32 heading;
  s32 startBehavior;
  FIXED x;
  FIXED z;
  struct Actor *actor;
  struct Vector3 *next;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Vector3 *position;
  u8 *flags;
  s32 i;
  FIXED startX;
  FIXED startZ;

  actor = Actor_Lookup(gMainRoutineState.leaderActorId);
  heading = (s16)FACING_NEAREST_CARDINAL(actor->facing);
  for (;;) {
    Leader_StopIfPending();
    actor->speedLimit = toFIXED(1);
    actor->acceleration = toFIXED(1) / 2;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    position = &probe;
    position->x = startX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    position->y = actor->y.fixed;
    position->z = startZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    x = startX;
    z = startZ;
    layer = &actor->layer;
    startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, startX, startZ);
    FieldVector_AddPolarOffset(16, heading, position);
    nextBehavior =
        FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        Map_GetTerrainHeight(*layer, position->x, position->z) -
                actor->y.fixed >
            toFIXED(8)) {
      actor->facing = heading;
      actor->velocityX = 0;
      actor->velocityZ = 0;
      actor->x.fixed = startX;
      actor->z.fixed = startZ;
      goto done;
    }
    position->x = x;
    position->z = z;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 10;
    actor->timer = 0;
    Actor_SelectAnimation(actor, ANIM_WALK);
    Actor_SetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
    actor->update = Actor_SteerByPad;
    for (;;) {
      FieldVector_AddPolarOffset(16, heading, &probe);
      next = &probe;
      nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
      if (nextBehavior == CELL_BEHAVIOR_WALL) {
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
        for (i = 0; i < ACTOR_COUNT;
             i++, flags += sizeof(struct Actor), other++) {
          if (other->script != NULL && (*flags & ACTOR_SOLID) &&
              other != actor) {
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
  }

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor->acceleration = toFIXED(1) / 4;
  Actor_SelectAnimation(actor, ANIM_STAND);
}
