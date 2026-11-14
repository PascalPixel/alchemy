/// Flame_UpdateForward for the rides that set the direction through their work.
void Flame_UpdateLanding(struct Actor *object) {
  struct FlameHitWork *work = (struct FlameHitWork *)(gInventorySnapshot + 0x188);
  struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  struct RideDirection *direction;

  object->z += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->x += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->sprite)->unk12 += object->timer;
  if (other->y == other->terrainHeight && Actor_IsTouching(object, other) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->direction.z = 0;
      direction = &work->direction;
      if (object->unk4c > 0)
        direction->x = 1;
      else
        direction->x = 0xffff;
    }
    object->priorityFlags = 2;
    object->z -= object->bounce;
    object->gravity += object->bounce >> 2;
    object->unk4c = 0;
    work->active = 1;
    event->stepTrigger = work->value0a;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->slots[i].id;

    if (id == 0)
      break;
    other = Actor_Get(id);
    if (other->y == other->terrainHeight && Actor_IsBeside(object, other)) {
      if (work->slots[i].blocked == 0) {
        object->priorityFlags = 2;
        object->z -= object->unk4c;
        object->gravity += object->unk4c >> 2;
        object->unk4c = 0;
      } else {
        work->blocked = 1;
      }
    }
  }
}
