/// @file
/// The motion of an object a vent throws out, shared by overlays 678 and 67C.
/// The including file has its overlay's local header (struct VentWork,
/// VENT_WORK, DIFF) and names the two functions: VENT_TOUCHING and
/// VENT_BOUNCE.

/// Whether object a rests on object b: within 12 pixels of it in x and 8 in z,
/// and above it by less than 16 pixels.
s32 VENT_TOUCHING(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x.fixed, b->x.fixed) < toFIXED(12)) {
    if ((u32)(a->y.fixed - b->y.fixed - 1) <= toFIXED(16) - 2) {
      if (DIFF(a->z.fixed, b->z.fixed) < toFIXED(8))
        return 1;
    }
  }
  return 0;
}

/// Moves the object by its velocities, slows it in x and z, grows and spins
/// its sprite, and stops its fall in z when it lands on the leader or on one
/// of the objects the vent work lists.
void VENT_BOUNCE(struct Actor *actor) {
  struct VentWork *work = VENT_WORK;
  struct Actor *other = Actor_GetById(gMainRoutineWork.leaderActorId);
  FIXED dx = actor->bounce;
  FIXED dz;
  struct Sprite *sprite;
  s32 i;

  actor->x.fixed += dx;
  actor->y.fixed += actor->gravity;
  dz = actor->riseBaseY;
  actor->z.fixed += dz;
  actor->bounce = dx - dx / 18;
  actor->riseBaseY = dz - dz / 16;
  actor->scaleX += actor->speedLimit;
  sprite = actor->sprite;
  {
    s32 acceleration = actor->acceleration;

    actor->scaleY += acceleration;
  }
  sprite->rotation += actor->timer;
  if (other->y.fixed == other->groundHeight && VENT_TOUCHING(actor, other)) {
    actor->z.fixed -= actor->riseBaseY;
    actor->gravity += actor->riseBaseY >> 2;
    actor->riseBaseY = 0;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->ids[i];

    if (id == 0)
      break;
    other = Actor_GetById(id);
    if (other->y.fixed == other->groundHeight && VENT_TOUCHING(actor, other)) {
      actor->z.fixed -= actor->riseBaseY;
      actor->gravity += actor->riseBaseY >> 2;
      actor->riseBaseY = 0;
    }
  }
}
