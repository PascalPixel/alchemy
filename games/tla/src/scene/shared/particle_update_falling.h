/// A falling rock's flight, in frames.
#define FALLING_ROCK_FRAMES 20

/// Each frame, the falling rocks in flight: each falls, damped and pulled down,
/// for FALLING_ROCK_FRAMES frames, keeping all but a sixty-fourth of its speed
/// each frame, and shows the five pictures of falling_rock.png in turn, four
/// frames apiece, shrinking as it goes.
void Particle_UpdateFalling(void) {
  struct ParticleState *state = Ram_HeapSlots->menuState;
  struct Particle *particle = state->particles;
  struct ParticleSprite *sprite = state->sprites;
  s32 i;
  FIXED step = -(toFIXED(1) / 2);

  for (i = ARRAY_COUNT(state->particles) - 1; i >= 0;
       i--, sprite++, particle++) {
    if ((u32)particle->variant <= FALLING_ROCK_FRAMES - 1) {
      u16 *base = &state->tile;
      s32 mask = 7;
      s32 tile = (u16)((*base + ((particle->variant >> 2) & mask) *
                                    SPRITE_TILES_8BPP(16, 16)) &
                       OBJ_TILE_MASK);
      s32 attr = sprite->attr2;

      sprite->attr2 = (attr & ~OBJ_TILE_MASK) | tile;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_StepVector3(particle, DAMPING_SCALE - 1, step);
      particle->variant++;
    }
  }
}

#undef FALLING_ROCK_FRAMES
