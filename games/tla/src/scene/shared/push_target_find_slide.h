/// Finds how far the object ahead slides before something blocks it; 1 when it
/// moves.
s32 PushTarget_FindSlide(struct PushTarget *target) {
  struct Actor *object;
  s32 direction;
  s32 position[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = PushTarget_FindAhead(&direction, &target->id, &target->footprint);
  if (object != 0) {
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
    layer = &object->terrainId;
    *layer = 2;
    target->x = object->x + dx;
    target->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = target->z + (gPushFootprints[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gPushFootprints[footprint][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_02002338_. */
          if (PushTarget_CheckCollision(object, position) == 2)
            goto blocked;
          position[0] += 0x100000;
        }
        position[2] += 0x100000;
      }
      target->x += dx;
      count++;
      target->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    target->x = object->x + dx * count;
    target->y = object->y;
    target->z = object->z + dz * count;
    return 1;
  }
  return 0;
}
