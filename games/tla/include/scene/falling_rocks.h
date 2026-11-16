#ifndef ALCHEMY_FALLING_ROCKS_H
#define ALCHEMY_FALLING_ROCKS_H

/// @file
/// Falling rocks and particles: the work Prox, Magma Rock, Eastern Alhafra and
/// Tundaria Tower each keep a copy of the code for.

#include "fixed_math.h"
#include "types.h"
#include "field/particle.h"

struct EffectSprite;

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling particle's sprite: struct EffectSprite with OAM attribute 2 read
/// whole, as the particles change its tile.
struct ParticleSprite {
  struct EffectSprite *next;
  u16 attr0;
  u16 attr1;
  u16 attr2; ///< bits: 15..12 palette | 11..10 priority | 9..0 tile
  u16 attr3;
  FIXED x;
  FIXED y;
  FIXED scaleX;
  FIXED scaleY;
  u16 angle;
  s16 depth;
  u16 width;
  u16 height;
  s16 pivotX;
  s16 pivotY;
};

/// The falling particles' work, in the menu work's heap slot: their tiles, 64
/// particles and their sprites, the next particle to start and the first
/// sprite tile their tiles were loaded to.
struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

#endif
