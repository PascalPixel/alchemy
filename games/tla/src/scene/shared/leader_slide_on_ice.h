/// The leader slides cell by cell over ice until something stops it.
void TundariaTower_SlideOnIce(void) {
  struct Vector3 probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Vector3 *position;
  struct Vector3 *next;
  s32 heading;
  s32 nextBehavior;
  s32 distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 dx;
  s32 dz;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);

top:
  TundariaTower_StopLeader();
  actor->speedLimit = px(1);
  actor->acceleration = px(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + px(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + px(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z;
  dx = x - actor->x;
  distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);
  if (distance < px(4)) {
    heading = gTundariaTower_SlideAnglesByKey[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  FieldVector_AddPolarOffset(px(16), heading, &probe);
  nextBehavior =
      FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          px(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  actor->timer = 0;
  FieldActor_SetMoveTargetFixed(actor, startX, actor->y, startZ);
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  FieldActor_WaitForMove(actor);
  actor->update = TundariaTower_KeepSliding;
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > px(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
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
      if (Field_CheckSpheresOverlap(&other->x, radius, &probe,
                                    other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = px(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTargetFixed(actor, next->x, next->y, next->z);
  FieldActor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  {
    struct Vector3 *step;

    step = &probe;
    FieldVector_AddPolarOffset(px(16), heading, step);
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the
     * address. */
    step = NULL;
  }
  next = &probe;
  nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = px(2);
  actor->acceleration = px(1);
  FieldActor_SetMoveTargetFixed(actor, x, actor->y, z);
  FieldActor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = px(1) / 4;
  Actor_SelectAnimation(actor, 1);
}
