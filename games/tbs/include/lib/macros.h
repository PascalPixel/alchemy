#ifndef ALCHEMY_MACROS_H
#define ALCHEMY_MACROS_H

/// @file
/// Small statement macros both games use for positions, velocities and
/// bounds.
// The multi-part ones are comma expressions or a plain block, not
// do { } while (0): a do-while (0) compiles differently at some of their sites
// (games/tbs/src/MATCHING.md, "Macros and their expansions").

/// Sets the x, y and z of a three-part vector: a position about to be
/// projected, a per-frame step, a camera point or a scale.
#define SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

/// Sets the position or the velocity of a particle, a piece of debris or an
/// object in one statement.
#define SET_POSITION(p, px, py, pz)                                            \
  ((p)->x = (px), (p)->y = (py), (p)->z = (pz))
#define SET_VELOCITY(p, vx, vy, vz)                                            \
  ((p)->velocityX = (vx), (p)->velocityY = (vy), (p)->velocityZ = (vz))

/// Keeps `v` within low to high, the low bound tested first. A plain block,
/// so it cannot stand before an `else`.
#define CLAMP(v, low, high)                                                    \
  {                                                                            \
    if ((v) < (low))                                                           \
      (v) = (low);                                                             \
    if ((v) > (high))                                                          \
      (v) = (high);                                                            \
  }

/// The magnitude of v. v is read twice.
#define ABS(v) ((v) < 0 ? -(v) : (v))

#endif
