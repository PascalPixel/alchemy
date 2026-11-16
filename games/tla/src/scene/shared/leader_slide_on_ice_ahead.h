/// Leader_SlideOnIce in the direction the leader faces.
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

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);
  heading = (s16)((actor->facing + 0x2000) & -0x4000);

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
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, startX, startZ);
  FieldVector_AddPolarOffset(16, heading, position);
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
  position->x = x;
  position->z = z;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 10;
  actor->timer = 0;
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Actor_SteerByPad;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y.fixed >
      toFIXED(8)) {
    goto stop;
  }
  {
    s32 radius = actor->radius - 2;
    other = Ram_HeapSlots->actors;
    flags = &other->collisionFlags;
    for (i = 0; i < 64; i++, flags += 128, other++) {
      if (other->script != NULL && (*flags & 1) && other != actor) {
        /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
         * next and actor. */
        asm(""
            :
            : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
              "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
            : SFP_CLOBBER, "r4");
        if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &probe,
                                      other->radius - 2) >= 0) {
          goto stop;
        }
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 10;
  FieldActor_SetMoveTargetFX(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  FieldVector_AddPolarOffset(16, heading, &probe);
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
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
