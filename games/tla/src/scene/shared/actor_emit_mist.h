/// Mist rises from an actor every other frame.
s32 Actor_EmitMist(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 x;
    s32 y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16() << 3) >> 16) * 0x3333;
    x = object->x;
    y = object->y;
    FieldEffect_Spawn(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000,
                      object->z, 0, -drift, 0, 0xb0000, params);
  }
  return 0;
}
