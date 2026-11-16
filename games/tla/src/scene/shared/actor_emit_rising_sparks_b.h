/// Actor_EmitRisingSparks with the spark settings in a plain array.
s32 Actor_EmitRisingSparksB(struct Actor *actor) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    FIXED x;
    FIXED y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = actor->x.fixed + (((Random_Below(9)) - 4) << 16);
    y = actor->y.fixed + ((32 - (Random_Below(20))) << 16);
    FieldEffect_Spawn(x, y, actor->z.fixed, 0,
                      (s32)(((Random_Below(5)) << 16) + 0x50000) / 10, 0,
                      0xb0000, params);
  }
  return 0;
}
