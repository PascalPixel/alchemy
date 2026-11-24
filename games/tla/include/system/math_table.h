#ifndef ALCHEMY_MATH_TABLE_H
#define ALCHEMY_MATH_TABLE_H

/// @file
/// The arctangent table, and beside it the fixed-point square root and the
/// basic sprite palettes, which The Broken Seal declares in system/system.h.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"

/// Tangents in 8.8 fixed point over a quarter turn in 256 steps, which
/// Trig_Atan searches.
extern const u16 gMath_TanTable[258];
/// The basic sprite palettes, loaded for the debug text
/// (Palette_GetBasicSpriteColors).
extern const u8 gSystem_BasicColorPalette[];
FIXED Math_SqrtFX(FIXED value);

#endif
