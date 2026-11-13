#ifndef ALCHEMY_MACROS_H
#define ALCHEMY_MACROS_H

/// @file
/// Small statement macros both games use for positions, velocities and
/// bounds.

/// Sets the x, y and z of a three-part vector: a position about to be
/// projected, a per-frame step, a camera point or a scale.
#define SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

/// Sets the position or the velocity of a particle, a piece of debris or
/// anything else with plain x, y and z in one statement (SET_ACTOR_POSITION
/// sets an actor's).
#define SET_POSITION(p, newX, newY, newZ)                                      \
  ((p)->x = (newX), (p)->y = (newY), (p)->z = (newZ))
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

/// 1 when `v` is not zero and 0 when it is, without a branch: the sign bit of
/// `v | -v`. The flag readers, the save checks and the Psynergy tests return
/// their truth values so; the name is ours. `v` is read twice.
#define NONZERO(v) ((u32)(-(v) | (v)) >> 31)

#endif
