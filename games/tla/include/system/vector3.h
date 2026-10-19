#ifndef ALCHEMY_VECTOR3_H
#define ALCHEMY_VECTOR3_H

#include "types.h"
#include "static_assert.h"

/// Three coordinates, x, y and z: a position, a velocity or a direction.
struct Vector3 {
#ifdef UBFIX
  union {
    struct {
      s32 x;
      s32 y;
      s32 z;
    };
    s32 coordinates[3]; ///< x, y and z, as Vector3_Coordinates returns them
  };
#else
  s32 x;
  s32 y;
  s32 z;
#endif
};

/// Returns the vector's three coordinates as an array.
// UB: transform routines index three coordinates from separate scalar members.
// UBFIX gives those routines a real array while preserving named components.
static inline const s32 *Vector3_Coordinates(const struct Vector3 *vector) {
#ifdef UBFIX
  return vector->coordinates;
#else
  return &vector->x;
#endif
}

STATIC_ASSERT(sizeof(struct Vector3) == 0x0c);
STATIC_ASSERT(offsetof(struct Vector3, x) == 0x00);
STATIC_ASSERT(offsetof(struct Vector3, y) == 0x04);
STATIC_ASSERT(offsetof(struct Vector3, z) == 0x08);

#endif
