#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

/// @file
/// The step counts and value shift of a curve table.

#include "types.h"

enum {
  /// Turns a height sample, in 8-pixel units, into 16.16 fixed-point pixels.
  CURVE_VALUE_SHIFT = 19,
  CURVE_HALF_STEPS = 8,  ///< pixels to the middle of a 16-pixel cell
  CURVE_FULL_STEPS = 16, ///< pixels across a cell
};

#endif
