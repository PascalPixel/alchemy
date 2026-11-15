/// Updates the falling particles and their sprites.
void Particles_UpdateFalling(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -(px(1) / 2);

  i = 63;
next:
  if ((u32)particle->variant <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->variant >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    BattleEffect_StepVector3(particle, 63, step);
    particle->variant++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}
