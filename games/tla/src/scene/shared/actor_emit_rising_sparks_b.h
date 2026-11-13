/// Actor_EmitRisingSparks with the spark settings in a plain array.
s32 Actor_EmitRisingSparksB(struct Actor *object) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = object->x + ((((Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16() * 20) >> 16)) << 16);
    FieldEffect_Spawn(x, y, object->z, 0,
                      (s32)((((Random16() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                      0xb0000, params);
  }
  return 0;
}
