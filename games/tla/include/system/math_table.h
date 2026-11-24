#ifndef ALCHEMY_MATH_TABLE_H
#define ALCHEMY_MATH_TABLE_H

/// @file
/// The arctangent table.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"

/// Tangents in 8.8 fixed point over a quarter turn in 256 steps, which
/// Trig_Atan searches.
extern const u16 gMath_TanTable[258];

#endif
