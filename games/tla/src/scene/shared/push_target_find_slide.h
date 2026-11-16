/// What MapGrid_CheckCollision says of a step to a position, 2 for a wall:
/// The Broken Seal's system_call.h layer.
static inline s32 FieldActor_CheckMovementCollision(struct Actor *actor,
                                                    s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

/// Finds how far the object ahead slides before something blocks it; 1 when it
/// moves.
s32 PushTarget_FindSlide(struct PushTarget *target) {
  struct Actor *actor;
  s32 direction;
  FIXED position[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 dx;
  FIXED dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  actor = PushTarget_FindAhead(&direction, &target->id, &target->footprint);
  if (actor != 0) {
    footprint = target->footprint;
    width = (ABS(gPushFootprints[footprint][0]) +
             ABS(gPushFootprints[footprint][2])) >>
            4;
    height = (ABS(gPushFootprints[footprint][1]) +
              ABS(gPushFootprints[footprint][3])) >>
             4;
    dx = gPushLeaderSteps[direction] & 0xffff0000;
    dz = gPushLeaderSteps[direction] << 16;
    target->callback = 0;
    count = 0;
    layer = &actor->layer;
    *layer = 2;
    target->x = actor->x.fixed + dx;
    target->z = actor->z.fixed + dz;
    position[1] = actor->y.fixed;
    for (;;) {
      position[2] = target->z + (gPushFootprints[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gPushFootprints[footprint][0] << 16);
        for (col = 0; col < width; col++) {
          if (FieldActor_CheckMovementCollision(actor, position) == 2)
            goto blocked;
          position[0] += toFIXED(16);
        }
        position[2] += toFIXED(16);
      }
      target->x += dx;
      count++;
      target->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    target->x = actor->x.fixed + dx * count;
    target->y = actor->y.fixed;
    target->z = actor->z.fixed + dz * count;
    return 1;
  }
  return 0;
}
