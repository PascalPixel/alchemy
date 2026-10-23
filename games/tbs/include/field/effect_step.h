#ifndef ALCHEMY_EFFECT_STEP_H
#define ALCHEMY_EFFECT_STEP_H

/// @file
/// Particle and ScreenPoint, the positions animations move through.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A particle of an animation: its position, its velocity and a value each
/// animation uses its own way (a size, a timer, -1 for idle).
struct Particle {
#ifdef UBFIX
  union {
    struct {
      s32 x; ///< 16.16 fixed-point pixels
      s32 y; ///< 16.16 fixed-point pixels
      s32 z; ///< 16.16 fixed-point pixels
    };
    s32 coordinates[3];
  };
#else
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
#endif
  s32 velocityX; ///< 16.16 fixed-point pixels per frame
  s32 velocityY; ///< 16.16 fixed-point pixels per frame
  s32 velocityZ; ///< 16.16 fixed-point pixels per frame
  s32 variant;   ///< each animation's own: a size, a countdown, -1 for idle
};

/// Returns the particle's three coordinates as an array.
// UB: indexing from &x crosses the particle's separate scalar members. UBFIX
// supplies a real coordinate array without including motion state.
static inline const s32 *Particle_Coordinates(const struct Particle *step) {
#ifdef UBFIX
  return step->coordinates;
#else
  return &step->x;
#endif
}

/// A point projected onto the screen, with its depth.
struct ScreenPoint {
#ifdef UBFIX
  /// Projection writes three coordinates; animations read their named
  /// components.
  union {
    struct {
      s32 x;     ///< screen pixels
      s32 y;     ///< screen pixels
      s32 depth; ///< projected depth, larger further away
    };
    s32 coordinates[3];
  };
#else
  s32 x;     ///< screen pixels
  s32 y;     ///< screen pixels
  s32 depth; ///< projected depth, larger further away
#endif
};

/// Returns the position's three coordinates as an array.
// UB: indexing from &x crosses separate scalar members of the point.
// UBFIX provides a real array while keeping the animations' named coordinates.
// An always-on union changes native alias analysis in the effect/window code.
static inline s32 *ScreenPoint_Coordinates(struct ScreenPoint *position) {
#ifdef UBFIX
  return position->coordinates;
#else
  return &position->x;
#endif
}

STATIC_ASSERT(offsetof(struct Particle, x) == 0x00);
STATIC_ASSERT(offsetof(struct Particle, y) == 0x04);
STATIC_ASSERT(offsetof(struct Particle, z) == 0x08);
STATIC_ASSERT(offsetof(struct Particle, velocityX) == 0x0c);
STATIC_ASSERT(offsetof(struct Particle, velocityY) == 0x10);
STATIC_ASSERT(offsetof(struct Particle, velocityZ) == 0x14);
STATIC_ASSERT(offsetof(struct Particle, variant) == 0x18);
STATIC_ASSERT(sizeof(struct Particle) == 0x1c);
STATIC_ASSERT(sizeof(struct ScreenPoint) == 0x0c);

#endif
