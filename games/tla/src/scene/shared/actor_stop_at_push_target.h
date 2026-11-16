/// Stops an object at its push target, or at what blocks it.
void Actor_StopAtPushTarget(struct Actor *actor) {
  FIXED position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = actor->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  FieldVector_AddPolarOffset(16, angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0) {
    for (i = 0; i <= 5; i++) {
      if (((struct Sprite *)hit->sprite)->entries[0]->animationId ==
          gPushKinds[i])
        return;
    }
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                         actor->z.fixed);
  }
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  if (MapGrid_CheckCollision(actor, position) > 0)
    FieldActor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                         actor->z.fixed);
}
