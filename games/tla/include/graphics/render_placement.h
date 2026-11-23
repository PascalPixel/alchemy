#ifndef ALCHEMY_RENDER_PLACEMENT_H
#define ALCHEMY_RENDER_PLACEMENT_H

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

/// A sprite's screen position and its two vertical offsets, in 16.16 pixels.
struct SpritePlacement {
#ifdef UBFIX
  union {
    struct {
      FIXED x;
      FIXED height;
      FIXED y;
      FIXED groundHeight;
    };
    FIXED values[4];
  };
#else
  FIXED x;
  FIXED height;
  FIXED y;
  FIXED groundHeight;
#endif
};

/// Returns the placement's values as a writable array.
static inline s32 *SpritePlacement_Values(struct SpritePlacement *placement) {
#ifdef UBFIX
  return placement->values;
#else
  return &placement->x;
#endif
}

/// Horizontal and vertical sprite scale; 0x10000 is the original size.
struct SpriteScale {
  union {
    struct {
      FIXED x; ///< 16.16
      FIXED y; ///< 16.16
    };
    s32 values[2];
  };
};

STATIC_ASSERT(sizeof(struct SpritePlacement) == 16);
STATIC_ASSERT(offsetof(struct SpritePlacement, height) == 4);
STATIC_ASSERT(offsetof(struct SpritePlacement, y) == 8);
STATIC_ASSERT(offsetof(struct SpritePlacement, groundHeight) == 12);
STATIC_ASSERT(sizeof(struct SpriteScale) == 8);
STATIC_ASSERT(offsetof(struct SpriteScale, y) == 4);

#endif
