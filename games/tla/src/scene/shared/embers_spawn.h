/// Two embers fly up from a point and bounce away for 20 frames, then flicker
/// 20 more and are gone (gEmbers_Script); `speed` tips their throw, and the
/// rolling fireball passes its facing.
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  // XXX: embers, or chips of the ice the fireball breaks (SPRITE_WIND_STONE's
  // picture, painted SPRITE_COLOR_GRB)? A render of a Mars Lighthouse fireball
  // breaking ice would settle it.
  for (i = 0; i < 2; i++) {
    struct Actor *object = FieldActor_Create(SPRITE_WIND_STONE, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      FIXED scale;

      rise = speed + (Random16() >> 4) - (Random16() >> 4);
      angle = DEGtoANG(90) + rise;
      radius = Random16() * 8;
      size = Random16();
      Actor_StartScript(object, gEmbers_Script);
      Actor_SetSpriteFlags(object, 0);
      scale = (size >> 1) + (FIXED_ONE / 4);
      object->velocityY = rise + (toFIXED(5) / 4);
      object->velocityZ = Iwram_MulFX(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulFX(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = toFIXED(33) / 100;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = FIXED_ONE / 2;
      Actor_SetSpriteColorMode(object, SPRITE_COLOR_GRB);
    }
  }
}
