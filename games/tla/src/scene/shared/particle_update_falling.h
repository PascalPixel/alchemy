/// A falling rock's flight, in frames.
#define FALLING_ROCK_FRAMES 20
/// The speed a rock keeps each frame: all but a sixty-fourth.
#define FALLING_ROCK_DAMPING 63
/// Attribute 2's ten bits of tile number.
#define OBJ_TILE_MASK 0x3ff

/// Each frame, the falling rocks in flight: each falls, damped and pulled down,
/// for FALLING_ROCK_FRAMES frames, and shows the five pictures of
/// falling_rock.png in turn, four frames apiece, shrinking as it goes.
void Particle_UpdateFalling(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  FIXED step = -(toFIXED(1) / 2);

  for (i = ARRAY_COUNT(work->particles) - 1; i >= 0;
       i--, sprite++, particle++) {
    if ((u32)particle->variant <= FALLING_ROCK_FRAMES - 1) {
      u16 *base = &work->tile;
      s32 mask = 7;
      s32 tile = (u16)((*base + ((particle->variant >> 2) & mask) * 8) &
                       OBJ_TILE_MASK);
      s32 attr = sprite->attr2;

      sprite->attr2 = (attr & ~OBJ_TILE_MASK) | tile;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_StepVector3(particle, FALLING_ROCK_DAMPING, step);
      particle->variant++;
    }
  }
}

#undef FALLING_ROCK_FRAMES
#undef FALLING_ROCK_DAMPING
#undef OBJ_TILE_MASK
