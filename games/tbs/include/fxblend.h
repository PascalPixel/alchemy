#ifndef ALCHEMY_FXBLEND_H
#define ALCHEMY_FXBLEND_H

/// @file
/// LightningWork, the field's colour blend work.

#include "types.h"
#include "static_assert.h"

/// The storm's lightning work: the colours it flashes between and their
/// difference, the countdown to the next flash and whether the thunder is
/// loud. It is heap slot HEAP_SLOT_LIGHTNING (gLightningWork), which the
/// lightning and Weather_StartLightningFlash take.
struct LightningWork {
  /// The storm's dark colours (PALETTE_TINT_WARM), as the colour buffer's
  /// B, G and R channels: each flash fades back to them.
  s16 from[0x540];
  s16 to[0x540];    ///< the lit colours (PALETTE_TINT_PALE), as from
  s16 delta[0x540]; ///< the 12-frame steps from to back to from
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

STATIC_ASSERT(sizeof(struct LightningWork) == 0x1f88);
STATIC_ASSERT(offsetof(struct LightningWork, to) == 0xa80);
STATIC_ASSERT(offsetof(struct LightningWork, delta) == 0x1500);
STATIC_ASSERT(offsetof(struct LightningWork, timer) == 0x1f80);
STATIC_ASSERT(offsetof(struct LightningWork, loud) == 0x1f84);

#endif
