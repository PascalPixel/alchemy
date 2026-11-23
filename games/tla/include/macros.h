#ifndef ALCHEMY_MACROS_H
#define ALCHEMY_MACROS_H

/// @file
/// Small statement macros both games use for positions, velocities and
/// bounds.

/// Sets the x, y and z of a three-part vector: a position about to be
/// projected, a per-frame step, a camera point or a scale.
#define SET_VECTOR(v, x, y, z) ((v)[0] = (x), (v)[1] = (y), (v)[2] = (z))

/// Sets the position or the velocity of a particle, a piece of debris or an
/// actor in one statement.
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

/// The entries of an array whose size is declared, as pret's global.h has it.
#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

/// 1 when `v` is not zero and 0 when it is, without a branch: the sign bit of
/// `v | -v`. The flag readers and the Psynergy tests return their truth values
/// so. `v` is read twice.
#define NONZERO(v) ((u32)(-(v) | (v)) >> 31)

/// One more than `i` as a signed halfword, the step taken in the high half of
/// a word: the scenes step the halfword counters of their short searches so,
/// as PushTarget_FillFootprint does.
#define NEXT_HALFWORD(i) ((s32)(((i) << 16) + (1 << 16)) >> 16)

/// A scene's data that its overlay places after pictures linked between its
/// tables: the overlay's linker script places .data.late after the first
/// of them and .data.last after the next.
#define LATE_DATA __attribute__((section(".data.late")))
#define LAST_DATA __attribute__((section(".data.last")))

/// Data aligned as the ROM places it, where its own type would not.
#define ALIGNED(n) __attribute__((aligned(n)))

/// A global kept in its object's .bss in the order written, where a plain
/// one would be a common symbol the linker places in an order of its own.
#define BSS_DATA __attribute__((section(".bss")))

#endif
