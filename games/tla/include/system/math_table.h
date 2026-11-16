#ifndef ALCHEMY_MATH_TABLE_H
#define ALCHEMY_MATH_TABLE_H

/// @file
/// The arctangent table, the fixed-point square root and the basic colour
/// palette, which The Broken Seal declares in its system headers.
// XXX: the basic colour palette and the square root are not math tables; which
// of The Lost Age's system headers declares each, as The Broken Seal's do,
// would settle where they go.

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
