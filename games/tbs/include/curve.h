#ifndef ALCHEMY_CURVE_H
#define ALCHEMY_CURVE_H

/// @file
/// The step counts and value shift of a curve table.

#include "types.h"

enum {
  CURVE_VALUE_SHIFT = 19,
  CURVE_HALF_STEPS = 8,
  CURVE_FULL_STEPS = 16,
};

#endif
