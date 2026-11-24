#ifndef ALCHEMY_DUST_H
#define ALCHEMY_DUST_H

/// @file
/// Dust and ground particles the scenes draw by hand.

#include "fixed_math.h"
#include "types.h"
#include "field/weather.h"

/// The heap slot weather while the scenes' sparkles run (GroundParticles_Draw):
/// The Broken Seal's WeatherState, grown to 64 particles, with the place the
/// particles gather around.
struct GroundParticleState {
  s32 entry;    ///< the VRAM cache entry holding the particles' tiles
  s32 tileBase; ///< their first tile in VRAM
  struct WeatherParticle particles[64];
  /// The 16.16 position (x, height, z) new particles appear around; NULL
  /// keeps them where they fall.
  FIXED *leader;
  u8 unused[4]; ///< no code reads it
  s32 stopped;  ///< nonzero stops new sparkle bursts, as in WeatherState
};

#endif
