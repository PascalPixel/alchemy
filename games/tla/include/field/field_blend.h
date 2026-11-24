#ifndef ALCHEMY_FIELD_BLEND_H
#define ALCHEMY_FIELD_BLEND_H

/// @file
/// The field's colour blend state.

#include "types.h"
#include "static_assert.h"

/// The storm's lightning state: the colours it flashes between and their
/// difference, the countdown to the next flash and whether the thunder is
/// loud.
struct FieldBlendState {
  /// The storm's dark colours (PALETTE_TINT_WARM), as the colour buffer's
  /// B, G and R channels: each flash fades back to them.
  s16 from[1344];
  s16 to[1344];    ///< the lit colours (PALETTE_TINT_PALE), as from
  s16 delta[1344]; ///< the 12-frame steps from to back to from
  /// Frames to the next flash; below 0 stops the lightning.
  s16 timer;
  /// Nonzero arms the next flash a random 51 to 549 frames after each, with
  /// a thunderclap.
  s16 enabled;
  s16 loud;              ///< nonzero plays the louder thunderclap
  u8 unusedAfterLoud[2]; ///< no code names it
};

STATIC_ASSERT(sizeof(struct FieldBlendState) == 8072);
STATIC_ASSERT(offsetof(struct FieldBlendState, to) == 2688);
STATIC_ASSERT(offsetof(struct FieldBlendState, delta) == 5376);
STATIC_ASSERT(offsetof(struct FieldBlendState, timer) == 8064);
STATIC_ASSERT(offsetof(struct FieldBlendState, loud) == 8068);

#endif
