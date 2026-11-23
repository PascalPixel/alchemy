/// Flame_UpdateForward for a flame flying east or west: it goes along x, and a
/// leader it reaches is knocked back along x.
void Flame_UpdateLanding(struct Actor *actor) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *other = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  FIXED dx = actor->bounce;
  FIXED dz;
  s32 i;
  struct RideDirection *direction;

  actor->z.fixed += dx;
  actor->y.fixed += actor->gravity;
  dz = actor->riseBaseY;
  actor->x.fixed += dz;
  actor->bounce = dx - dx / 18;
  actor->riseBaseY = dz - dz / 16;
  actor->scaleX += actor->speedLimit;
  actor->scaleY += actor->acceleration;
  ((struct FieldSprite *)actor->sprite)->rotation += actor->timer;
  if (other->y.fixed == other->groundHeight && Actor_IsTouching(actor, other) &&
      gMainRoutineWork.movementMode == LEADER_MOVEMENT_WALK) {
    if (actor->riseBaseY != 0) {
      work->direction.z = 0;
      direction = &work->direction;
      if (actor->riseBaseY > 0)
        direction->x = 1;
      else
        direction->x = -1;
    }
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->z.fixed -= actor->bounce;
    actor->gravity += actor->bounce >> 2;
    actor->riseBaseY = 0;
    work->active = 1;
    event->stepTrigger = work->hitTrigger;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y.fixed == other->groundHeight && Actor_IsBeside(actor, other)) {
      if (work->slots[i].blocked == 0) {
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        actor->z.fixed -= actor->riseBaseY;
        actor->gravity += actor->riseBaseY >> 2;
        actor->riseBaseY = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}
