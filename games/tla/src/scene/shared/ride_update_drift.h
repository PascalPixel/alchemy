/// A drifting object's update, stopping against the leader.
void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->riseBaseY;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->riseBaseY = dz - dz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  parts->unk12 += object->timer;
  if (leader->y == leader->groundHeight && Actor_IsAbove(object, leader)) {
    object->z -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 2;
    object->riseBaseY = 0;
  }
}
