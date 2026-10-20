#ifndef ALCHEMY_FXBLEND_H
#define ALCHEMY_FXBLEND_H

/// @file
/// FieldBlendWork, the field's colour blend work.

#include "types.h"
#include "static_assert.h"

/// The storm's lightning work: the colours it flashes between and their
/// difference, the countdown to the next flash and whether the thunder is
/// loud. It is heap slot HEAP_SLOT_FIELD_BLEND (gFieldBlendWork), which the
/// lightning and BattleFx_StartBufferBlend take.
struct FieldBlendWork {
  /// The storm's dark colours (PALETTE_TRANSFORM_WARM), as the colour buffer's
  /// B, G and R channels: each flash fades back to them.
  s16 from[0x540];
  s16 to[0x540];    ///< the lit colours (PALETTE_TRANSFORM_PALE), as from
  s16 delta[0x540]; ///< the 12-frame steps from to back to from
  /// Frames to the next flash, which comes in three, 5 frames apart; below 0
  /// stops the lightning.
  s16 timer;
  /// Nonzero arms the next flash a random 51 to 549 frames after each, with
  /// a thunderclap.
  s16 enabled;
  s16 loud;      ///< nonzero plays the louder thunderclap, SE_172 for SE_171
  u8 unk1f86[2]; ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct FieldBlendWork) == 0x1f88);
STATIC_ASSERT(offsetof(struct FieldBlendWork, to) == 0xa80);
STATIC_ASSERT(offsetof(struct FieldBlendWork, delta) == 0x1500);
STATIC_ASSERT(offsetof(struct FieldBlendWork, timer) == 0x1f80);
STATIC_ASSERT(offsetof(struct FieldBlendWork, loud) == 0x1f84);

#endif
