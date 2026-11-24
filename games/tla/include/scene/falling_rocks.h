#ifndef ALCHEMY_FALLING_ROCKS_H
#define ALCHEMY_FALLING_ROCKS_H

/// @file
/// Falling rocks and particles: the effects Prox, Magma Rock, Eastern Alhafra
/// and Tundaria Tower each keep a copy of the code for.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "field/effect_sprite.h"
#include "field/particle.h"

/// The falling rocks in flight at once, the ring a scene's rock state keeps.
#define ROCK_COUNT 64
/// The falling rock's pictures, decompressed, in bytes.
#define ROCK_SHEET_BYTES 1280

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// The falling rocks' state, in the menu state's heap slot: their tiles, the
/// rocks and their sprites, in a ring that next walks round, the first sprite
/// tile their tiles were loaded to and the VRAM cache entry holding them.
struct RockState {
  u8 tiles[ROCK_SHEET_BYTES];
  struct Rock rocks[ROCK_COUNT];
  struct EffectSprite sprites[ROCK_COUNT];
  s16 next;
  u16 tileBase;
  s16 resource;
};

STATIC_ASSERT(offsetof(struct RockState, resource) == 5636);

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

/// The falling particles' state, in the menu state's heap slot: their tiles, 64
/// particles and their sprites, the next particle to start and the first
/// sprite tile their tiles were loaded to.
struct ParticleState {
  u8 tiles[ROCK_SHEET_BYTES];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

#endif
