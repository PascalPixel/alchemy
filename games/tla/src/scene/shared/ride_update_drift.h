/// A drifting object's update, stopping against the leader.
void Ride_UpdateDrift(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED dx = object->bounce;
  FIXED dz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x.fixed += dx;
  object->y.fixed += object->gravity;
  dz = object->riseBaseY;
  object->z.fixed += dz;
  object->bounce = dx - dx / 18;
  object->riseBaseY = dz - dz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  sprite->rotation += object->timer;
  if (leader->y.fixed == leader->groundHeight &&
      Actor_IsAbove(object, leader)) {
    object->z.fixed -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 2;
    object->riseBaseY = 0;
  }
}
