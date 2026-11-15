#ifndef ALCHEMY_MATH_TABLE_H
#define ALCHEMY_MATH_TABLE_H

/// @file
/// The arctangent table, the fixed-point square root and the basic colour
/// palette, which The Broken Seal declares in its system headers.
// XXX: the basic colour palette and the square root are not math tables; which
// of The Lost Age's system headers declares each, as The Broken Seal's do,
// would settle where they go.

#include "types.h"
#include "edition.h"

extern const u16 gMath_ArcTanTable[];
extern const u8 gSystem_BasicColorPalette[];
s32 Math_SqrtFX(s32 value);

#endif
