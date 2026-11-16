/// A drifting object's update, stopping against the leader.
void Ride_UpdateDrift(struct Actor *actor) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED dx = actor->bounce;
  FIXED dz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  actor->x.fixed += dx;
  actor->y.fixed += actor->gravity;
  dz = actor->riseBaseY;
  actor->z.fixed += dz;
  actor->bounce = dx - dx / 18;
  actor->riseBaseY = dz - dz / 16;
  rate = actor->speedLimit;
  scale = actor->scaleX;
  sprite = actor->sprite;
  actor->scaleX = scale + rate;
  actor->scaleY += actor->acceleration;
  sprite->rotation += actor->timer;
  if (leader->y.fixed == leader->groundHeight && Actor_IsAbove(actor, leader)) {
    actor->z.fixed -= actor->riseBaseY;
    actor->gravity += actor->riseBaseY >> 2;
    actor->riseBaseY = 0;
  }
}
