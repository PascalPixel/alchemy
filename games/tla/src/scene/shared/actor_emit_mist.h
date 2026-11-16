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
    params[2] = FIXED_ONE * 4 / 5;
    params[3] = FIXED_ONE * 4 / 5;
    params[0] = 0;
    drift = ((u32)(Random16() << 3) >> 16) * toFIXED(0.2);
    x = actor->x.fixed;
    y = actor->y.fixed;
    FieldEffect_Spawn(x + ((8 - (gFrameCount & 15)) << 16), y + toFIXED(26),
                      actor->z.fixed, 0, -drift, 0, toFIXED(11), params);
  }
  return 0;
}
