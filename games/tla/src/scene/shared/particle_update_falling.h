/// Updates the falling particles and their sprites.
void Particle_UpdateFalling(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  FIXED step = -(toFIXED(1) / 2);

  for (i = 63; i >= 0; i--, sprite++, particle++) {
    if ((u32)particle->variant <= 19) {
      u16 *base = &work->tile;
      s32 mask = 7;
      s32 tile = (u16)((*base + ((particle->variant >> 2) & mask) * 8) & 0x3ff);
      s32 attr = sprite->attr2;

      sprite->attr2 = (attr & ~0x3ff) | tile;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_StepVector3(particle, 63, step);
      particle->variant++;
    }
  }
}
