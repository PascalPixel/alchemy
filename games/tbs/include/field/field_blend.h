#ifndef ALCHEMY_FIELD_BLEND_H
#define ALCHEMY_FIELD_BLEND_H

/// @file
/// FieldBlendState, the field's colour blend state.

#include "static_assert.h"
#include "types.h"

/// The storm's lightning state: the colours it flashes between and their
/// difference, the countdown to the next flash and whether the thunder is loud.
struct FieldBlendState {
  /// The storm's dark colours (PALETTE_TINT_DIM), as the palette fade's
  /// B, G and R channels: each flash fades back to them.
  s16 from[1344];
  s16 to[1344];    ///< the lit colours (PALETTE_TINT_PALE), as from
  s16 delta[1344]; ///< the 12-frame steps from to back to from
  /// Frames to the next flash, which comes in three, 5 frames apart; below 0
  /// stops the lightning.
  s16 timer;
  /// Nonzero arms the next flash a random 51 to 549 frames after each, with
  /// a thunderclap.
  s16 enabled;
  s16 loud;      ///< nonzero plays the louder thunderclap, SE_THUNDER_LOUD for
                 ///< SE_THUNDER
  u8 padding[2]; ///< up to the struct's word-aligned size
};

STATIC_ASSERT(sizeof(struct FieldBlendState) == 8072);
STATIC_ASSERT(offsetof(struct FieldBlendState, to) == 2688);
STATIC_ASSERT(offsetof(struct FieldBlendState, delta) == 5376);
STATIC_ASSERT(offsetof(struct FieldBlendState, timer) == 8064);
STATIC_ASSERT(offsetof(struct FieldBlendState, loud) == 8068);

#endif
