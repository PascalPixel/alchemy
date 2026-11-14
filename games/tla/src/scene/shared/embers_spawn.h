/// Two embers fly up from a point.
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(325, x, y, z);

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
      /* FAKEMATCH(temporary): the scale is taken before the velocity
       * stores. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = Q16_ONE / 2;
      Actor_SetSpriteColorMode(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}
