/// Stops a pushed thing where it is when an actor stands in the cell ahead of
/// it, unless that is another pushable thing, or when the map blocks it.
void Actor_StopAtPushTarget(struct Actor *actor) {
  FIXED position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = FACING_INDEX(actor->facing);

  // The facing rounded to the nearest of east, south, west and north: two
  // sixteenths on, whole quarters kept, and back from sixteenths to a facing.
  angle = FACING_FROM_INDEX((angle + 2) & 0xc);
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  FieldVector_AddPolarOffset(16, angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != 0) {
    for (i = 0; i <= PUSH_KINDS - 1; i++) {
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
