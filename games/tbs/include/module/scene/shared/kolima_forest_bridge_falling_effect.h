#ifndef ALCHEMY_MODULE_SCENE_SHARED_KOLIMA_FOREST_BRIDGE_FALLING_EFFECT_H
#define ALCHEMY_MODULE_SCENE_SHARED_KOLIMA_FOREST_BRIDGE_FALLING_EFFECT_H

#include "types.h"

/// gFallingEffectState: how the curse dust falls about the point the map view
/// follows (Kolima_UpdateCurseDust).
enum KolimaCurseDustState {
  KOLIMA_DUST_CENTRED = 0, ///< within 128 pixels of the point
  /// Over a band that widens, by 50 a frame to 15000, as it closes in.
  KOLIMA_DUST_CLOSING = 1,
  /// Over a band that widens, by 50 a frame to 30000, as it closes in nearer.
  KOLIMA_DUST_CLOSING_WIDE = 2,
  /// Drifting on until it is far past the point, then back to
  /// KOLIMA_DUST_CENTRED.
  KOLIMA_DUST_DRIFTING = 3
};

void Kolima_UpdateCurseDust(void);

#endif
