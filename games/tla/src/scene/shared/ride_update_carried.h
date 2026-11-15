/// A ridden object's update: it drifts and slows, and lands the leader where it
/// touches down.
void Flame_UpdateForward(struct Actor *object) {
  struct FlameHitWork *work =
      (struct FlameHitWork *)(gInventorySnapshot + 0x188);
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  FIXED dx = object->bounce;
  FIXED dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x.fixed += dx;
  object->y.fixed += object->gravity;
  dz = object->riseBaseY;
  object->z.fixed += dz;
  object->bounce = dx - dx / 18;
  object->riseBaseY = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (actor->y.fixed == actor->groundHeight &&
      Actor_IsTouching(object, actor) && gMainRoutineWork.movementMode == 0) {
    if (object->riseBaseY != 0) {
      work->direction.x = 0;
      {
        u16 *cell = &work->direction.z;

        if (object->riseBaseY > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    {
      s32 speed = object->riseBaseY;

      object->riseBaseY = 0;
      object->z.fixed -= speed;
      object->gravity += speed >> 2;
    }
    work->active = 1;
    event->stepTrigger = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y.fixed == actor->groundHeight &&
        Actor_IsBeside(object, actor)) {
      if (work->slots[i].blocked == 0) {
        FIXED speed;

        object->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        speed = object->riseBaseY;
        object->riseBaseY = 0;
        object->z.fixed -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}
