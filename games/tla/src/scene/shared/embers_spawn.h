/// Two embers fly up from a point.
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(SPRITE_TURNING_ROCK, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + (Random16() >> 4) - (Random16() >> 4);
      angle = 0x4000 + rise;
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
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}
