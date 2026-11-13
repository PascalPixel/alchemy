/// Stops an object at its push target, or at what blocks it.
void Actor_StopAtPushTarget(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 == gPushKinds[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (MapGrid_CheckCollision(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y,
                                         object->z);
}
