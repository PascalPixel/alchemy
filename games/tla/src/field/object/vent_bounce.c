/// @file
/// The motion of an object a vent throws out, shared by overlays 678 and 67C.
/// The including file has its overlay's local header (struct VentWork,
/// VENT_WORK, DIFF) and names the two functions: VENT_TOUCHING and
/// VENT_BOUNCE.

/// Whether object a rests on object b: within 0xc0000 of it in x and 0x80000
/// in z, and up to 0xfffff above it.
s32 VENT_TOUCHING(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y - 1) <= 0xffffe) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

/// Moves the object by its velocities, slows it in x and z, grows and spins
/// its sprite, and stops its fall in z when it lands on the leader or on one
/// of the objects the vent work lists.
void VENT_BOUNCE(struct Actor *object) {
  struct VentWork *work = VENT_WORK;
  struct Actor *other = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct FieldSprite *sprite;
  s32 i;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  sprite = object->animation;
  /* FAKEMATCH: a one-pass block with the acceleration read first; without it
   * sched2 copies the object for the spin's address before the scale store
   * (differ 2). */
  do {
    s32 acceleration = object->acceleration;

    object->scaleY += acceleration;
  } while (0);
  sprite->rotation += object->action;
  if (other->y == other->terrainHeight && VENT_TOUCHING(object, other)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
  for (i = 0; i < 4; i++) {
    s32 id = work->ids[i];

    if (id == 0)
      break;
    other = Actor_GetById(id);
    if (other->y == other->terrainHeight && VENT_TOUCHING(object, other)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
}
