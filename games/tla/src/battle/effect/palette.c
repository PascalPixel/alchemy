/// @file
/// Palette: clamp RGB555 components.

#include "types.h"

/// Clamps a five-bit component to 0-31.
s32 Graphics_ClampRgb555Channel(s32 val) {
  if (val > 31)
    return 31;
  if (val < 0)
    val = 0;
  return val;
}

/// Clamps to an upper bound of 0x7c00 (RGB555 maximum blue), with no lower
/// bound.
s32 Graphics_ClampRgb555Component(s32 val) {
  if (val > 0x7c00)
    val = 0x7c00;
  return val;
}
