#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "fixed_math.h"
#include "types.h"
#include "field/sprite.h"

struct FadeActor;

/// The VRAM cache entry holding the ground mask's solid sprite tiles.
#define VRAM_CACHE_ENTRY_FADE_OVERLAY 94

/// The fade overlay's work (heap slot fadeOverlay): its two marks and the
/// actor they follow.
struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

/// A ground particle: an OAM entry linked by depth, its place (16.16 pixels),
/// its delay and its frame count.
struct GroundParticle {
  u8 unused[4]; ///< no code names it
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2]; ///< no code names it
  FIXED posX;
  FIXED posY;
  FIXED posZ;
  s32 delay;
  u16 timer;
  u8 unusedAfterTimer[2]; ///< no code names it
};

/// GroundParticle.timer after the decrement that counts a live particle down,
/// when it was already free (0): the u16 wraps.
#define GROUND_PARTICLE_FREED 65535

/// WeatherWork.particles.
#define WEATHER_PARTICLE_COUNT 64
/// A raindrop's frames (Weather_UpdateRain).
#define RAIN_DROP_FRAMES 12

/// The heap slot weather while the ground particles run: their tiles' VRAM
/// cache entry and first tile, and the particles.
struct WeatherWork {
  s32 entry; ///< the VRAM cache entry holding the particles' tiles
  /* FAKEMATCH(signature): the tile base typed long; as s32. */
  long tileBase; ///< their first tile in VRAM
  struct GroundParticle particles[WEATHER_PARTICLE_COUNT];
  u8 unused[8]; ///< no code reads them
  s32 stopped;  ///< nonzero stops new sparkle bursts
};

#endif
