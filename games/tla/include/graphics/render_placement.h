#ifndef ALCHEMY_RENDER_PLACEMENT_H
#define ALCHEMY_RENDER_PLACEMENT_H

#include "types.h"
#include "static_assert.h"

/// A sprite's screen position and its two vertical offsets, in 16.16 pixels.
struct SpritePlacement {
#ifdef UBFIX
  union {
    struct {
      s32 x;
      s32 height;
      s32 y;
      s32 groundHeight;
    };
    s32 values[4];
  };
#else
  s32 x;
  s32 height;
  s32 y;
  s32 groundHeight;
#endif
};

/// Returns the placement's values as an array.
// UB: the legacy renderer walks across the scalar members as an array. UBFIX
// supplies a real array; an always-on union changes producer scheduling.
static inline const s32 *
SpritePlacement_ReadValues(const struct SpritePlacement *placement) {
#ifdef UBFIX
  return placement->values;
#else
  return &placement->x;
#endif
}

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
      s32 x; ///< 16.16
      s32 y; ///< 16.16
    };
    s32 values[2];
  };
};

STATIC_ASSERT(sizeof(struct SpritePlacement) == 0x10);
STATIC_ASSERT(offsetof(struct SpritePlacement, height) == 0x04);
STATIC_ASSERT(offsetof(struct SpritePlacement, y) == 0x08);
STATIC_ASSERT(offsetof(struct SpritePlacement, groundHeight) == 0x0c);
STATIC_ASSERT(sizeof(struct SpriteScale) == 0x08);
STATIC_ASSERT(offsetof(struct SpriteScale, y) == 0x04);

#endif
