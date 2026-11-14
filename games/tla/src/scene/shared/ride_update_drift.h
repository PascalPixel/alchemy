/// A drifting object's update, stopping against the leader.
void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  parts->unk12 += object->timer;
  if (leader->y == leader->terrainHeight && Actor_IsAbove(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}
