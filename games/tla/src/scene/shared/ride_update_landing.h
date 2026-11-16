/// Flame_UpdateForward for the rides that set the direction through their work.
void Flame_UpdateLanding(struct Actor *object) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *other = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  FIXED dx = object->bounce;
  FIXED dz;
  s32 i;
  struct RideDirection *direction;

  object->z.fixed += dx;
  object->y.fixed += object->gravity;
  dz = object->riseBaseY;
  object->x.fixed += dz;
  object->bounce = dx - dx / 18;
  object->riseBaseY = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y.fixed == other->groundHeight &&
      Actor_IsTouching(object, other) && gMainRoutineWork.movementMode == 0) {
    if (object->riseBaseY != 0) {
      work->direction.z = 0;
      direction = &work->direction;
      if (object->riseBaseY > 0)
        direction->x = 1;
      else
        direction->x = 0xffff;
    }
    object->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    object->z.fixed -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->riseBaseY = 0;
    work->active = 1;
    event->stepTrigger = work->hitTrigger;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y.fixed == other->groundHeight &&
        Actor_IsBeside(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        object->z.fixed -= object->riseBaseY;
        object->gravity += object->riseBaseY >> 2;
        object->riseBaseY = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}
