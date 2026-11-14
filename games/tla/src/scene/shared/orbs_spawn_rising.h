void Orbs_SpawnRising(void) {
  struct Actor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_SCENE_TEMP_01) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_LOWER_BEAM);
  if (Event_GetFlag(FLAG_SCENE_TEMP_01) != 0) {
    u32 random;

    random = Random16();
    effect = CreateOrbitEffect(0x2a0, center->x.fixed,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y.fixed - px(28),
                               center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = CreateOrbitEffect(0x2a0, center->x.fixed,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y.fixed - px(28),
                               center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gOrbScript);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Orb_Update;
    effect->radius = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}
