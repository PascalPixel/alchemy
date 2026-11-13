#ifndef ALCHEMY_MACROS_H
#define ALCHEMY_MACROS_H

/// @file
/// Small statement macros both games use for positions, velocities and
/// bounds.
// The multi-part ones are comma expressions or a plain block, not
// do { } while (0): a do-while (0) compiles differently at some of their
// sites (see AGENTS.md, S2).

/// Sets the x, y and z of a three-part vector: a position about to be
/// projected, a per-frame step, a camera point or a scale.
#define SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

/// Set the position or the velocity of a particle, a piece of debris or an
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

/// A scene's data that its overlay places after pictures linked between its
/// tables: the overlay's linker script places .data.late after the first
/// of them and .data.last after the next.
#define LATE_DATA __attribute__((section(".data.late")))
#define LAST_DATA __attribute__((section(".data.last")))

/// Data aligned as the ROM places it, where its own type would not.
#define ALIGNED(n) __attribute__((aligned(n)))

#endif
