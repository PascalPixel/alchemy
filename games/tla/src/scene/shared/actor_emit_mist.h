/// Mist rises from an actor every other frame.
s32 Actor_EmitMist(struct Actor *actor) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    FIXED x;
    FIXED y;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16() << 3) >> 16) * 0x3333;
    x = actor->x.fixed;
    y = actor->y.fixed;
    FieldEffect_Spawn(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000,
                      actor->z.fixed, 0, -drift, 0, toFIXED(11), params);
  }
  return 0;
}
