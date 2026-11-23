/// What MapGrid_CheckCollision says of a step to a position (enum Collision),
/// under The Broken Seal's name for it.
static inline s32 FieldActor_CheckMovementCollision(struct Actor *actor,
                                                    s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

/// Finds where the actor ahead stops when pushed: a cell at a time, the
/// whole of its rectangle (gPushRectangles) until a wall blocks it; 1 when it
/// moves at all.
s32 PushTarget_FindStop(struct PushTarget *target) {
  struct Actor *actor;
  s32 direction;
  FIXED position[3];
  s32 rectangle;
  s32 width;
  s32 height;
  s32 dx;
  FIXED dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  actor = PushTarget_FindAhead(&direction, &target->id, &target->rectangle);
  if (actor != 0) {
    rectangle = target->rectangle;
    width = (ABS(gPushRectangles[rectangle][0]) +
             ABS(gPushRectangles[rectangle][2])) >>
            4;
    height = (ABS(gPushRectangles[rectangle][1]) +
              ABS(gPushRectangles[rectangle][3])) >>
             4;
    // A push step holds x pixels in its high half and z in its low.
    dx = gPushLeaderSteps[direction] & FIELD_PIXEL_MASK;
    dz = INTtoFIXED(gPushLeaderSteps[direction]);
    target->callback = 0;
    count = 0;
    layer = &actor->layer;
    *layer = MAPGRID_LAYER_OVERHEAD;
    target->x = actor->x.fixed + dx;
    target->z = actor->z.fixed + dz;
    position[1] = actor->y.fixed;
    for (;;) {
      position[2] = target->z + INTtoFIXED(gPushRectangles[rectangle][1]);
      for (row = 0; row < height; row++) {
        position[0] = target->x + INTtoFIXED(gPushRectangles[rectangle][0]);
        for (col = 0; col < width; col++) {
          if (FieldActor_CheckMovementCollision(actor, position) ==
              COLLISION_IMPASSABLE)
            goto blocked;
          position[0] += toFIXED(MAPGRID_CELL_PIXELS);
        }
        position[2] += toFIXED(MAPGRID_CELL_PIXELS);
      }
      target->x += dx;
      count++;
      target->z += dz;
    }
  blocked:
    *layer = MAPGRID_LAYER_GROUND;
    if (count == 0)
      return 0;
    target->x = actor->x.fixed + dx * count;
    target->y = actor->y.fixed;
    target->z = actor->z.fixed + dz * count;
    return 1;
  }
  return 0;
}
